/*
 * Copyright (c) 1998, 1999, 2000, 2001, 2002 X-Way Rights BV
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*!\file entropy.c
 * \author Bob Deblier <bob.deblier@telenet.be>
 * \ingroup ES_m ES_audio_m ES_dsp_m ES_random_m ES_urandom_m ES_tty_m
 */

#define BEECRYPT_DLL_EXPORT

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "beecrypt/entropy.h"
#include "beecrypt/endianness.h"

#if WIN32
# include <mmsystem.h>
# include <wincrypt.h>
# include <winerror.h>
#else 
# if HAVE_SYS_IOCTL_H
#  include <sys/ioctl.h>
# endif
# if HAVE_SYS_STAT_H
#  include <sys/types.h>
#  include <sys/stat.h>
# endif
# if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
# else
#  if HAVE_SYS_TIME_H
#   include <sys/time.h>
#  elif HAVE_TIME_H
#   include <time.h>
#  endif
# endif
# if HAVE_SYS_AUDIOIO_H
#   include <sys/audioio.h>
# endif
# if HAVE_SYS_SOUNDCARD_H
#  include <sys/soundcard.h>
# endif
# if HAVE_TERMIOS_H
#  include <termios.h>
# elif HAVE_TERMIO_H
#  include <termio.h>
# endif
# ifdef _REENTRANT
#  if HAVE_THREAD_H && HAVE_SYNCH_H
#   include <synch.h>
#  elif HAVE_PTHREAD_H
#   include <pthread.h>
#  endif
# endif
# if HAVE_AIO_H
#  include <aio.h>
# endif
#endif
#if HAVE_FCNTL_H
# include <fcntl.h>
#endif
#if HAVE_ERRNO_H
# include <errno.h>
#endif

#if WIN32
static HINSTANCE	entropy_instance = (HINSTANCE) 0;

static HANDLE entropy_wavein_lock;
static HANDLE entropy_wavein_event;

int entropy_provider_setup(HINSTANCE hInst)
{
	if (!entropy_instance)
	{
		entropy_instance = hInst;
		if (!(entropy_wavein_lock = CreateMutex(NULL, FALSE, NULL)))
			return -1;
		if (!(entropy_wavein_event = CreateEvent(NULL, FALSE, FALSE, NULL)))
			return -1;
	}
	return 0;
}

int entropy_provider_cleanup()
{
	if (entropy_wavein_lock)
	{
		CloseHandle(entropy_wavein_lock);
		entropy_wavein_lock = 0;
	}
	if (entropy_wavein_event)
	{
		CloseHandle(entropy_wavein_event);
		entropy_wavein_event = 0;
	}
	return 0;
}
#endif

#if WIN32 || HAVE_DEV_AUDIO || HAVE_DEV_DSP
/*
 * Mask the low-order bit of a bunch of sound samples, analyze them and
 * return an error in case they are all zeroes or ones.
 */
static int entropy_noise_filter(void* sampledata, int samplecount, int samplesize, int channels, int swap)
{
	register int rc = 0, i;

	switch (samplesize)
	{
	case 1:
		{
			uint8_t* samples = (uint8_t*) sampledata;

			switch (channels)
			{
			case 1:
				{
					int zero_count = 0;
					int ones_count = 0;

					for (i = 0; i < samplecount; i++)
					{
						if (samples[i] &= 0x1)
							ones_count++;
						else
							zero_count++;
					}

					if ((zero_count == 0) || (ones_count == 0))
					{
						#if HAVE_ERRNO_H
						errno = EIO;
						#endif
						rc = -1;
					}
				}
				break;

			case 2:
				{
					int zero_count_left = 0;
					int ones_count_left = 0;
					int zero_count_right = 0;
					int ones_count_right = 0;

					for (i = 0; i < samplecount; i++)
					{
						if (i & 1)
						{
							if (samples[i] &= 0x1)
								ones_count_left++;
							else
								zero_count_left++;
						}
						else
						{
							if (samples[i] &= 0x1)
								ones_count_right++;
							else
								zero_count_right++;
						}
					}

					if ((zero_count_left == 0) || (ones_count_left == 0) ||
							(zero_count_right == 0) || (ones_count_right == 0))
					{
						#if HAVE_ERRNO_H
						errno = EIO;
						#endif
						rc = -1;
					}
				}
				break;

			default:
				#if HAVE_ERRNO_H
				errno = EINVAL;
				#endif
				rc = -1;
			}
		}
		break;

	case 2:
		{
			uint16_t* samples = (uint16_t*) sampledata;

			switch (channels)
			{
			case 1:
				{
					int zero_count = 0;
					int ones_count = 0;

					for (i = 0; i < samplecount; i++)
					{
						if (swap)
							samples[i] = swapu16(samples[i]);

						if (samples[i] &= 0x1)
							ones_count++;
						else
							zero_count++;
					}

					if ((zero_count == 0) || (ones_count == 0))
					{
						#if HAVE_ERRNO_H
						errno = EIO;
						#endif
						rc = -1;
					}
				}
				break;

			case 2:
				{
					int zero_count_left = 0;
					int ones_count_left = 0;
					int zero_count_right = 0;
					int ones_count_right = 0;

					for (i = 0; i < samplecount; i++)
					{
						if (swap)
							samples[i] = swapu16(samples[i]);

						if (i & 1)
						{
							if (samples[i] &= 0x1)
								ones_count_left++;
							else
								zero_count_left++;
						}
						else
						{
							if (samples[i] &= 0x1)
								ones_count_right++;
							else
								zero_count_right++;
						}
					}

					if ((zero_count_left == 0) || (ones_count_left == 0) ||
							(zero_count_right == 0) || (ones_count_right == 0))
					{
						#if HAVE_ERRNO_H
						errno = EIO;
						#endif
						rc = -1;
					}
				}
				break;

			default:
				#if HAVE_ERRNO_H
				errno = EINVAL;
				#endif
				rc = -1;
			}
		}
		break;

	default:
		#if HAVE_ERRNO_H
		errno = EINVAL;
		#endif
		rc = -1;
	}

	return 0;
}

/* bit deskewing technique: the classical Von Neumann method
	- only use the lsb bit of every sample
	- there is a chance of bias in 0 or 1 bits, so to deskew this:
		- look at two successive sampled bits
		- if they are the same, discard them
		- if they are different, they're either 0-1 or 1-0; use the first bit of the pair as output
*/

#if WIN32
static int entropy_noise_gather(HWAVEIN wavein, int samplesize, int channels, int swap, int timeout, byte* data, size_t size)
#else
static int entropy_noise_gather(int fd, int samplesize, int channels, int swap, int timeout, byte* data, size_t size)
#endif
{
	size_t randombits = size << 3;
	byte temp = 0;
	int rc, i;

	byte* sampledata = (byte*) malloc(1024 * samplesize * channels);

	#if WIN32
	WAVEHDR header;

	/* first set up a wave header */
	header.lpData = (LPSTR) sampledata;
	header.dwBufferLength = 1024 * samplesize * channels;
	header.dwFlags = 0;

	/* do error handling! */
	waveInStart(wavein);
	
	/* the first event is the due to the opening of the wave */
	ResetEvent(entropy_wavein_event);
	#else
	# if ENABLE_AIO
	struct aiocb my_aiocb;
	const struct aiocb* my_aiocb_list = &my_aiocb;
	#  if HAVE_TIME_H
	struct timespec my_aiocb_timeout;
	#  else
	#   error
	#  endif

	memset(&my_aiocb, 0, sizeof(struct aiocb));

	my_aiocb.aio_fildes = fd;
	my_aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
	# endif
	#endif

	if (sampledata == (byte*) 0)
	{
		#if HAVE_ERRNO_H
		errno = ENOMEM;
		#endif
		return -1;
	}

	while (randombits)
	{
		#if WIN32
		/* pass the buffer to the wavein and wait for the event */
		waveInPrepareHeader(wavein, &header, sizeof(WAVEHDR));
		waveInAddBuffer(wavein, &header, sizeof(WAVEHDR));

		/* in case we have to wait more than the specified timeout, bail out */
		if (WaitForSingleObject(entropy_wavein_event, timeout) == WAIT_OBJECT_0)
		{
			rc = header.dwBytesRecorded;
		}
		else
		{
			waveInStop(wavein);
			waveInReset(wavein);

			free(sampledata);
			return -1;
		}
		#else
		# if ENABLE_AIO
		my_aiocb.aio_buf = sampledata;
		my_aiocb.aio_nbytes = 1024 * samplesize * channels;

		rc = aio_read(&my_aiocb);
		# else
		rc = read(fd, sampledata, 1024 * samplesize * channels);
		# endif

		if (rc < 0)
		{
			free(sampledata);
			return -1;
		}

		# if ENABLE_AIO
		my_aiocb_timeout.tv_sec = (timeout / 1000);
		my_aiocb_timeout.tv_nsec = (timeout % 1000) * 1000000;

		rc = aio_suspend(&my_aiocb_list, 1, &my_aiocb_timeout);

		if (rc < 0)
		{
			#if HAVE_ERRNO_H
			if (errno == EAGAIN)
			{
				/* certain linux glibc versions are buggy and don't aio_suspend properly */
				nanosleep(&my_aiocb_timeout, (struct timespec*) 0);

				my_aiocb_timeout.tv_sec = (timeout / 1000);
				my_aiocb_timeout.tv_nsec = (timeout % 1000) * 1000000;

				/* and try again */
				rc = aio_suspend(&my_aiocb_list, 1, &my_aiocb_timeout);
			}
			#endif
		}

		if (rc < 0)
		{
			/* cancel any remaining reads */
			while (rc != AIO_ALLDONE)
			{
				rc = aio_cancel(fd, (struct aiocb*) 0);

				if (rc == AIO_NOTCANCELED)
				{
					my_aiocb_timeout.tv_sec = (timeout / 1000);
					my_aiocb_timeout.tv_nsec = (timeout % 1000) * 1000000;

					nanosleep(&my_aiocb_timeout, (struct timespec*) 0);
				}

				if (rc < 0)
					break;
			}
			free(sampledata);
			return -1;
		}

		rc = aio_error(&my_aiocb);

		if (rc)
		{
			free(sampledata);
			return -1;
		}

		rc = aio_return(&my_aiocb);

		if (rc < 0)
		{
			free(sampledata);
			return -1;
		}
		# endif
		#endif

		if (entropy_noise_filter(sampledata, rc / samplesize, samplesize, channels, swap) < 0)
		{
			fprintf(stderr, "noise filter indicates too much bias in audio samples\n");
			free(sampledata);
			return -1;
		}

		switch (samplesize)
		{
		case 1:
			{
				uint8_t* samples = (uint8_t*) sampledata;

				for (i = 0; randombits && (i < 1024); i += 2)
				{
					if (samples[i] ^ samples[i+1])
					{
						temp <<= 1;
						temp |= samples[i];
						randombits--;
						if (!(randombits & 0x7))
							*(data++) = temp;
					}
				}
			}
			break;

		case 2:
			{
				uint16_t* samples = (uint16_t*) sampledata;

				for (i = 0; randombits && (i < 1024); i += 2)
				{
					if (samples[i] ^ samples[i+1])
					{
						temp <<= 1;
						temp |= samples[i];
						randombits--;
						if (!(randombits & 0x7))
							*(data++) = temp;
					}
				}
			}
			break;

		default:
			free(sampledata);
			return -1;
		}
	}

	#if WIN32
	waveInStop(wavein);
	waveInReset(wavein);
	#endif

	free(sampledata);
	return 0;
}
#endif

#if WIN32
int entropy_wavein(byte* data, size_t size)
{
	const char *timeout_env = getenv("BEECRYPT_ENTROPY_WAVEIN_TIMEOUT");

	WAVEINCAPS		waveincaps;
	WAVEFORMATEX	waveformatex;
	HWAVEIN			wavein;
	MMRESULT		rc;
	
	rc = waveInGetDevCaps(WAVE_MAPPER, &waveincaps, sizeof(WAVEINCAPS));
	if (rc != MMSYSERR_NOERROR)
		return -1;

	/* first go for the 16 bits samples -> more chance of noise bits */
	switch (waveformatex.nChannels = waveincaps.wChannels)
	{
	case 1:
		/* mono */
		if (waveincaps.dwFormats & WAVE_FORMAT_4M16)
		{
			waveformatex.nSamplesPerSec = 44100;
			waveformatex.wBitsPerSample = 16;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_2M16)
		{
			waveformatex.nSamplesPerSec = 22050;
			waveformatex.wBitsPerSample = 16;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_1M16)
		{
			waveformatex.nSamplesPerSec = 11025;
			waveformatex.wBitsPerSample = 16;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_4M08)
		{
			waveformatex.nSamplesPerSec = 44100;
			waveformatex.wBitsPerSample = 8;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_2M08)
		{
			waveformatex.nSamplesPerSec = 22050;
			waveformatex.wBitsPerSample = 8;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_1M08)
		{
			waveformatex.nSamplesPerSec = 11025;
			waveformatex.wBitsPerSample = 8;
		}
		else
			return -1;
			
		break;
	case 2:
		/* stereo */
		if (waveincaps.dwFormats & WAVE_FORMAT_4S16)
		{
			waveformatex.nSamplesPerSec = 44100;
			waveformatex.wBitsPerSample = 16;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_2S16)
		{
			waveformatex.nSamplesPerSec = 22050;
			waveformatex.wBitsPerSample = 16;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_1S16)
		{
			waveformatex.nSamplesPerSec = 11025;
			waveformatex.wBitsPerSample = 16;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_4S08)
		{
			waveformatex.nSamplesPerSec = 44100;
			waveformatex.wBitsPerSample = 8;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_2S08)
		{
			waveformatex.nSamplesPerSec = 22050;
			waveformatex.wBitsPerSample = 8;
		}
		else if (waveincaps.dwFormats & WAVE_FORMAT_1S08)
		{
			waveformatex.nSamplesPerSec = 11025;
			waveformatex.wBitsPerSample = 8;
		}
		else
			return -1;

		break;
	}
	
	waveformatex.wFormatTag = WAVE_FORMAT_PCM;
	waveformatex.nAvgBytesPerSec = (waveformatex.nSamplesPerSec * waveformatex.nChannels * waveformatex.wBitsPerSample) / 8;
	waveformatex.nBlockAlign = (waveformatex.nChannels * waveformatex.wBitsPerSample) / 8;
	waveformatex.cbSize = 0;

	/* we now have the wavein's capabilities hammered out; from here on we need to lock */

	if (WaitForSingleObject(entropy_wavein_lock, INFINITE) != WAIT_OBJECT_0)
		return -1;

	rc = waveInOpen(&wavein, WAVE_MAPPER, &waveformatex, (DWORD_PTR) entropy_wavein_event, (DWORD) 0, CALLBACK_EVENT);
	if (rc != MMSYSERR_NOERROR)
	{
		fprintf(stderr, "waveInOpen failed!\n"); fflush(stderr);
		ReleaseMutex(entropy_wavein_lock);
		return -1;
	}

	rc = entropy_noise_gather(wavein, waveformatex.wBitsPerSample >> 3, waveformatex.nChannels, 0, timeout_env ? atoi(timeout_env) : 2000, data, size);

	waveInClose(wavein);

	ReleaseMutex(entropy_wavein_lock);

	return rc;
}

int entropy_console(byte* data, size_t size)
{
	register size_t randombits = size << 3;

	HANDLE hStdin;
	DWORD inRet;
	INPUT_RECORD inEvent;
	LARGE_INTEGER hrtsample;

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "GetStdHandle error %d\n", GetLastError());
		return -1;
	}

	printf("please press random keys on your keyboard\n"); fflush(stdout);

	while (randombits)
	{
		if (!ReadConsoleInput(hStdin, &inEvent, 1, &inRet))
		{
			fprintf(stderr, "ReadConsoleInput failed\n"); fflush(stderr);
			return -1;
		}
		if ((inRet == 1) && (inEvent.EventType == KEY_EVENT) && inEvent.Event.KeyEvent.bKeyDown)
		{
			printf("."); fflush(stdout);
			if (!QueryPerformanceCounter(&hrtsample))
			{
				fprintf(stderr, "QueryPerformanceCounter failed\n"); fflush(stderr);
				return -1;
			}

			/* get 8 bits from the sample */
			/* discard the 2 lowest bits */
			*(data++) = (byte)(hrtsample.LowPart >> 2);
			randombits -= 8;
		}
	}

	printf("\nthanks\n");

	Sleep(1000);
	
	if (!FlushConsoleInputBuffer(hStdin))
	{
		fprintf(stderr, "FlushConsoleInputBuffer failed\n"); fflush(stderr);
		return -1;
	}

	return 0;
}

int entropy_wincrypt(byte* data, size_t size)
{
	HCRYPTPROV hCrypt;
	DWORD provType = PROV_RSA_FULL;
	BOOL rc;

	/* consider using getenv("BEECRYPT_ENTROPY_WINCRYPT_PROVTYPE") to set provType */

	if (!CryptAcquireContext(&hCrypt, "BeeCrypt", NULL, provType, 0))
	{
		#if defined(NTE_BAD_KEYSET)
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			if (!CryptAcquireContext(&hCrypt, "BeeCrypt", NULL, provType, CRYPT_NEWKEYSET))
				return -1;
		}
		else
			return -1;
		#else
		return -1;
		#endif
	}

	rc = CryptGenRandom(hCrypt, size, (BYTE*) data);

	CryptReleaseContext(hCrypt, 0);

	return rc ? 0 : -1;
}

#else

#if HAVE_DEV_AUDIO
/*!\addtogroup ES_audio_m
 * \{
 */
static const char* name_dev_audio = "/dev/audio";
static int dev_audio_fd = -1;
# ifdef _REENTRANT
#  if HAVE_THREAD_H && HAVE_SYNCH_H
static mutex_t dev_audio_lock = DEFAULTMUTEX;
#  elif HAVE_PTHREAD_H
static pthread_mutex_t dev_audio_lock = PTHREAD_MUTEX_INITIALIZER;
#  else
#   error Need locking mechanism
#  endif
# endif
/*!\}
 */
#endif

#if HAVE_DEV_DSP
/*!\addtogroup ES_dsp_m
 * \{
 */
static const char* name_dev_dsp = "/dev/dsp";
static int dev_dsp_fd = -1;
# ifdef _REENTRANT
#  if HAVE_THREAD_H && HAVE_SYNCH_H
static mutex_t dev_dsp_lock = DEFAULTMUTEX;
#  elif HAVE_PTHREAD_H
static pthread_mutex_t dev_dsp_lock = PTHREAD_MUTEX_INITIALIZER;
#  else
#   error Need locking mechanism
#  endif
# endif
/*!\}
 */
#endif

#if HAVE_DEV_RANDOM
/*!\addtogroup ES_random_m
 * \{
 */
static const char* name_dev_random = "/dev/random";
static int dev_random_fd = -1;
# ifdef _REENTRANT
#  if HAVE_THREAD_H && HAVE_SYNCH_H
static mutex_t dev_random_lock = DEFAULTMUTEX;
#  elif HAVE_PTHREAD_H
static pthread_mutex_t dev_random_lock = PTHREAD_MUTEX_INITIALIZER;
#  else
#   error Need locking mechanism
#  endif
# endif
/*!\}
 */
#endif

#if HAVE_DEV_URANDOM
/*!\addtogroup ES_urandom_m
 * \{
 */
static const char* name_dev_urandom = "/dev/urandom";
static int dev_urandom_fd = -1;
# ifdef _REENTRANT
#  if HAVE_THREAD_H && HAVE_SYNCH_H
static mutex_t dev_urandom_lock = DEFAULTMUTEX;
#  elif HAVE_PTHREAD_H
static pthread_mutex_t dev_urandom_lock = PTHREAD_MUTEX_INITIALIZER;
#  else
#   error Need locking mechanism
#  endif
# endif
/*!\}
 */
#endif

#if HAVE_DEV_TTY
/*!\addtogroup ES_tty_m
 * \{
 */
static const char *dev_tty_name = "/dev/tty";
static int dev_tty_fd = -1;
# ifdef _REENTRANT
#  if HAVE_THREAD_H && HAVE_SYNCH_H
static mutex_t dev_tty_lock = DEFAULTMUTEX;
#  elif HAVE_PTHREAD_H
static pthread_mutex_t dev_tty_lock = PTHREAD_MUTEX_INITIALIZER;
#  else
#   error Need locking mechanism
#  endif
# endif
/*!\}
 */
#endif

#if HAVE_SYS_STAT_H
static int statdevice(const char *device)
{
	struct stat s;

	if (stat(device, &s) < 0)
	{
		#if HAVE_ERRNO_H && HAVE_STRING_H
		fprintf(stderr, "cannot stat %s: %s\n", device, strerror(errno));
		#endif
		return -1;
	}
	if (!S_ISCHR(s.st_mode))
	{
		fprintf(stderr, "%s is not a device\n", device);
		return -1;
	}
	return 0;
}
#endif

static int opendevice(const char *device)
{
	register int fd;

	if ((fd = open(device, O_RDONLY)) < 0)
	{
		#if HAVE_ERRNO_H && HAVE_STRING_H
		fprintf(stderr, "open of %s failed: %s\n", device, strerror(errno));
		#endif
		return fd;
	}
	
	return fd;
}

#if HAVE_DEV_RANDOM || HAVE_DEV_URANDOM
/* timeout is in milliseconds */
/*!\ingroup ES_random_m ES_urandom_m
 */
static int entropy_randombits(int fd, int timeout, byte* data, size_t size)
{
	register int rc;

	#if ENABLE_AIO
	struct aiocb my_aiocb;
	const struct aiocb* my_aiocb_list = &my_aiocb;
	# if HAVE_TIME_H
	struct timespec my_aiocb_timeout;
	# else
	#  error
	# endif

	memset(&my_aiocb, 0, sizeof(struct aiocb));

	my_aiocb.aio_fildes = fd;
	my_aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
	#endif

	while (size)
	{
		#if ENABLE_AIO
		my_aiocb.aio_buf = data;
		my_aiocb.aio_nbytes = size;

		rc = aio_read(&my_aiocb);
		#else
		rc = read(fd, data, size);
		#endif

		if (rc < 0)
			return -1;

		#if ENABLE_AIO
		my_aiocb_timeout.tv_sec = (timeout / 1000);
		my_aiocb_timeout.tv_nsec = (timeout % 1000) * 1000000;

		rc = aio_suspend(&my_aiocb_list, 1, &my_aiocb_timeout);

		if (rc < 0)
		{
			#if HAVE_ERRNO_H
			if (errno == EAGAIN)
			{
				/* certain linux glibc versions are buggy and don't aio_suspend properly */
				nanosleep(&my_aiocb_timeout, (struct timespec*) 0);

				my_aiocb_timeout.tv_sec = 0;
				my_aiocb_timeout.tv_nsec = 0;

				/* and try again */
				rc = aio_suspend(&my_aiocb_list, 1, &my_aiocb_timeout);
			}
			#endif
		}

		if (rc < 0)
		{
			/* cancel any remaining reads */
			while (rc != AIO_ALLDONE)
			{
				rc = aio_cancel(fd, (struct aiocb*) 0);

				if (rc == AIO_NOTCANCELED)
				{
					my_aiocb_timeout.tv_sec = (timeout / 1000);
					my_aiocb_timeout.tv_nsec = (timeout % 1000) * 1000000;

					nanosleep(&my_aiocb_timeout, (struct timespec*) 0);
				}

				if (rc < 0)
					break;
			}

			return -1;
		}

		rc = aio_error(&my_aiocb);

		if (rc < 0)
			return -1;

		rc = aio_return(&my_aiocb);

		if (rc < 0)
			return -1;
		#endif

		data += rc;
		size -= rc;
	}
	return 0;
}
#endif

#if HAVE_DEV_TTY
/*!\ingroup ES_tty_m
 */
static int entropy_ttybits(int fd, byte* data, size_t size)
{
	byte dummy;

	#if HAVE_TERMIOS_H
	struct termios tio_save, tio_set;
	#elif HAVE_TERMIO_H
	struct termio tio_save, tio_set;
	#else
	# error need alternative
	#endif
	#if HAVE_GETHRTIME
	hrtime_t hrtsample;
	#elif HAVE_GETTIMEOFDAY
	struct timeval tvsample;
	#else
	# error need alternative high-precision timer
	#endif

	printf("please press random keys on your keyboard\n");

	#if HAVE_TERMIOS_H
	if (tcgetattr(fd, &tio_save) < 0)
	{
		#if HAVE_ERRNO_H
		perror("tcgetattr failed");
		#endif
		return -1;
	}

	tio_set = tio_save;
	tio_set.c_cc[VMIN] = 1;				/* read 1 tty character at a time */
	tio_set.c_cc[VTIME] = 0;			/* don't timeout the read */
	tio_set.c_iflag |= IGNBRK;			/* ignore <ctrl>-c */
	tio_set.c_lflag &= ~(ECHO|ICANON);	/* don't echo characters */

	/* change the tty settings, and flush input characters */
	if (tcsetattr(fd, TCSAFLUSH, &tio_set) < 0)
	{
		#if HAVE_ERRNO_H
		perror("tcsetattr failed");
		#endif
		return -1;
	}
	#elif HAVE_TERMIO_H
	if (ioctl(fd, TCGETA, &tio_save) < 0)
	{
		#if HAVE_ERRNO_H
		perror("ioctl TCGETA failed");
		#endif
		return -1;
	}

	tio_set = tio_save;
	tio_set.c_cc[VMIN] = 1;				/* read 1 tty character at a time */
	tio_set.c_cc[VTIME] = 0;			/* don't timeout the read */
	tio_set.c_iflag |= IGNBRK;			/* ignore <ctrl>-c */
	tio_set.c_lflag &= ~(ECHO|ICANON);	/* don't echo characters */

	/* change the tty settings, and flush input characters */
	if (ioctl(fd, TCSETAF, &tio_set) < 0)
	{
		#if HAVE_ERRNO_H
		perror("ioctl TCSETAF failed");
		#endif
		return -1;
	}
	#else
	# error Need alternative tty control library
	#endif

	while (size)
	{
		if (read(fd, &dummy, 1) < 0)
		{
			#if HAVE_ERRNO_H
			perror("tty read failed");
			#endif
			return -1;
		}
		printf("."); fflush(stdout);
		#if HAVE_GETHRTIME
		hrtsample = gethrtime();
		/* discard the 10 lowest bits i.e. 1024 nanoseconds of a sample */
		*(data++) = (byte)(hrtsample >> 10);
		size--;
		#elif HAVE_GETTIMEOFDAY
		/* discard the 4 lowest bits i.e. 4 microseconds */
		gettimeofday(&tvsample, 0);
		/* get 8 bits from the sample */
		*(data) = (byte)(tvsample.tv_usec >> 2);
		size--;
		#else
		# error Need alternative high-precision timer sample
		#endif
	}

	printf("\nthanks\n");

	/* give the user 1 second to stop typing */
	sleep(1);

	#if HAVE_TERMIOS_H
	/* change the tty settings, and flush input characters */
	if (tcsetattr(fd, TCSAFLUSH, &tio_save) < 0)
	{
		#if HAVE_ERRNO_H
		perror("tcsetattr failed");
		#endif
		return -1;
	}
	#elif HAVE_TERMIO_H
	/* restore the tty settings, and flush input characters */
	if (ioctl(fd, TCSETAF, &tio_save) < 0)
	{
		#if HAVE_ERRNO_H
		perror("ioctl TCSETAF failed");
		#endif
		return -1;
	}
	#else
	# error Need alternative tty control library
	#endif

	return 0;
}
#endif

#if HAVE_DEV_AUDIO
/*!\ingroup ES_audio_m
 */
int entropy_dev_audio(byte* data, size_t size)
{
	const char* timeout_env = getenv("BEECRYPT_ENTROPY_AUDIO_TIMEOUT");

	register int rc;

	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	if (mutex_lock(&dev_audio_lock))
		return -1;
	# elif HAVE_PTHREAD_H
	if (pthread_mutex_lock(&dev_audio_lock))
		return -1;
	# endif
	#endif

	#if HAVE_SYS_STAT_H
	if (statdevice(name_dev_audio) < 0)
		goto dev_audio_end;
	#endif
	
	if ((rc = dev_audio_fd = opendevice(name_dev_audio)) < 0)
		goto dev_audio_end;

	#if HAVE_SYS_AUDIOIO_H /* i.e. Solaris */
	{
		struct audio_info info;

		AUDIO_INITINFO(&info);

		info.record.sample_rate = 48000;
		info.record.channels = 2;
		info.record.precision = 16;
		info.record.encoding = AUDIO_ENCODING_LINEAR;
		info.record.gain = AUDIO_MAX_GAIN;
		info.record.pause = 0;
		info.record.buffer_size = 4096;
		info.record.samples = 0;

		if ((rc = ioctl(dev_audio_fd, AUDIO_SETINFO, &info)) < 0)
		{
			if (errno == EINVAL)
			{
				/* use a conservative setting this time */
				info.record.sample_rate = 22050;
				info.record.channels = 1;
				info.record.precision = 8;

				if ((rc = ioctl(dev_audio_fd, AUDIO_SETINFO, &info)) < 0)
				{
					#if HAVE_ERRNO_H
					perror("ioctl AUDIO_SETINFO failed");
					#endif
					close(dev_audio_fd);

					goto dev_audio_end;
				}
			}
			else
			{
				#if HAVE_ERRNO_H
				perror("ioctl AUDIO_SETINFO failed");
				#endif
				close(dev_audio_fd);

				goto dev_audio_end;
			}
		}

		rc = entropy_noise_gather(dev_audio_fd, info.record.precision >> 3, info.record.channels, 0, timeout_env ? atoi(timeout_env) : 1000, data, size);
	}
	#else
	# error Unknown type of /dev/audio interface
	#endif

	close(dev_audio_fd);

dev_audio_end:
	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	mutex_unlock(&dev_audio_lock);
	# elif HAVE_PTHREAD_H
	pthread_mutex_unlock(&dev_audio_lock);
	# endif
	#endif
	return rc;
}
#endif

#if HAVE_DEV_DSP
/*!\ingroup ES_dsp_m
 */
int entropy_dev_dsp(byte* data, size_t size)
{
	const char* timeout_env = getenv("BEECRYPT_ENTROPY_DSP_TIMEOUT");

	register int rc;

	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	if (mutex_lock(&dev_dsp_lock))
		return -1;
	# elif HAVE_PTHREAD_H
	if (pthread_mutex_lock(&dev_dsp_lock))
		return -1;
	# endif
	#endif

	#if HAVE_SYS_STAT_H
	if ((rc = statdevice(name_dev_dsp)) < 0)
		goto dev_dsp_end;
	#endif
	
	if ((rc = dev_dsp_fd = opendevice(name_dev_dsp)) < 0)
		goto dev_dsp_end;

	#if HAVE_SYS_SOUNDCARD_H /* i.e. Linux audio */
	{
		int mask, format, samplesize, stereo, speed, swap;

		if ((rc = ioctl(dev_dsp_fd, SNDCTL_DSP_GETFMTS, &mask)) < 0)
		{
			#if HAVE_ERRNO_H
			perror("ioctl SNDCTL_DSP_GETFMTS failed");
			#endif
			close (dev_dsp_fd);

			goto dev_dsp_end;
		}

		#if WORDS_BIGENDIAN
		if (mask & AFMT_S16_BE)
		{
			format = AFMT_S16_BE;
			samplesize = 2;
			swap = 0;
		}
		else if (mask & AFMT_S16_LE)
		{
			format = AFMT_S16_LE;
			samplesize = 2;
			swap = 1;
		}
		#else
		if (mask & AFMT_S16_LE)
		{
			format = AFMT_S16_LE;
			samplesize = 2;
			swap = 0;
		}
		else if (mask & AFMT_S16_BE)
		{
			format = AFMT_S16_BE;
			samplesize = 2;
			swap = 1;
		}
		#endif
		else if (mask & AFMT_S8)
		{
			format = AFMT_S8;
			samplesize = 1;
			swap = 0;
		}
		else
		{
			/* No linear audio format available */
			rc = -1;

			close(dev_dsp_fd);

			goto dev_dsp_end;
		}

		if ((rc = ioctl(dev_dsp_fd, SNDCTL_DSP_SETFMT, &format)) < 0)
		{
			#if HAVE_ERRNO_H
			perror("ioctl SNDCTL_DSP_SETFMT failed");
			#endif
			close(dev_dsp_fd);

			goto dev_dsp_end;
		}

		/* the next two commands are not critical */
		stereo = 1;
		ioctl(dev_dsp_fd, SNDCTL_DSP_STEREO, &stereo);

		speed = 44100;
		ioctl(dev_dsp_fd, SNDCTL_DSP_SPEED, &speed);

		rc = entropy_noise_gather(dev_dsp_fd, samplesize, 2, swap, timeout_env ? atoi(timeout_env) : 1000, data, size);
	}
	#else
	# error Unknown type of /dev/dsp interface
	#endif

	close(dev_dsp_fd);

dev_dsp_end:
	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	mutex_unlock(&dev_dsp_lock);
	# elif HAVE_PTHREAD_H
	pthread_mutex_unlock(&dev_dsp_lock);
	# endif
	#endif

	return rc;
}
#endif

#if HAVE_DEV_RANDOM
/*!\ingroup ES_random_m
 */
int entropy_dev_random(byte* data, size_t size)
{
	const char* timeout_env = getenv("BEECRYPT_ENTROPY_RANDOM_TIMEOUT");

	int rc;

	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	if (mutex_lock(&dev_random_lock))
		return -1;
	# elif HAVE_PTHREAD_H
	if (pthread_mutex_lock(&dev_random_lock))
		return -1;
	# endif
	#endif

	#if HAVE_SYS_STAT_H
	if ((rc = statdevice(name_dev_random)) < 0)
		goto dev_random_end;
	#endif

	if ((rc = dev_random_fd = opendevice(name_dev_random)) < 0)
		goto dev_random_end;

	/* collect entropy, with timeout */
	rc = entropy_randombits(dev_random_fd, timeout_env ? atoi(timeout_env) : 1000, data, size);

	close(dev_random_fd);

dev_random_end:
	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	mutex_unlock(&dev_random_lock);
	# elif HAVE_PTHREAD_H
	pthread_mutex_unlock(&dev_random_lock);
	# endif
	#endif
	return rc;
}
#endif

#if HAVE_DEV_URANDOM
/*!\ingroup ES_urandom_m
 */
int entropy_dev_urandom(byte* data, size_t size)
{
	const char* timeout_env = getenv("BEECRYPT_ENTROPY_URANDOM_TIMEOUT");

	register int rc;

	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	if (mutex_lock(&dev_urandom_lock))
		return -1;
	# elif HAVE_PTHREAD_H
	if (pthread_mutex_lock(&dev_urandom_lock))
		return -1;
	# endif
	#endif

	#if HAVE_SYS_STAT_H
	if ((rc = statdevice(name_dev_urandom)) < 0)
		goto dev_urandom_end;
	#endif
	
	if ((rc = dev_urandom_fd = opendevice(name_dev_urandom)) < 0)
		goto dev_urandom_end;

	/* collect entropy, with timeout */
	rc = entropy_randombits(dev_urandom_fd, timeout_env ? atoi(timeout_env) : 1000, data, size);

	close(dev_urandom_fd);

dev_urandom_end:
	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	mutex_unlock(&dev_urandom_lock);
	# elif HAVE_PTHREAD_H
	pthread_mutex_unlock(&dev_urandom_lock);
	# endif
	#endif
	return rc;
}
#endif

#if HAVE_DEV_TTY
/*!\ingroup ES_tty_m
 */
int entropy_dev_tty(byte* data, size_t size)
{
	register int rc;

	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	if (mutex_lock(&dev_tty_lock))
		return -1;
	# elif HAVE_PTHREAD_H
	if (pthread_mutex_lock(&dev_tty_lock))
		return -1;
	# endif
	#endif

	#if HAVE_SYS_STAT_H
	if ((rc = statdevice(dev_tty_name)) < 0)
		goto dev_tty_end;
	#endif
	
	if ((rc = dev_tty_fd = opendevice(dev_tty_name)) < 0)
		goto dev_tty_end;

	rc = entropy_ttybits(dev_tty_fd, data, size);

	close(dev_tty_fd);

dev_tty_end:
	#ifdef _REENTRANT
	# if HAVE_THREAD_H && HAVE_SYNCH_H
	mutex_unlock(&dev_tty_lock);
	# elif HAVE_PTHREAD_H
	pthread_mutex_unlock(&dev_tty_lock);
	# endif
	#endif

	return rc;
}
#endif

#endif

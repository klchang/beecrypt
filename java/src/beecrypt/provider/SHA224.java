package beecrypt.provider;

import java.security.*;
import java.nio.*;

public final class SHA224 extends MessageDigestSpi {
	private long param;

	private static native long allocParam();

	private static native long cloneParam(long param);

	private static native void freeParam(long param);

	private static native byte[] digest(long param);

	private static native int digest(long param, byte[] buf, int off, int len)
			throws DigestException;

	private static native void reset(long param);

	private static native void update(long param, byte input);

	private static native void update(long param, byte[] input, int off, int len);

	public SHA224() {
		param = allocParam();
	}

	public SHA224(SHA224 copy) {
		param = cloneParam(copy.param);
	}

	public Object clone() throws CloneNotSupportedException {
		return new SHA224(this);
	}

	protected byte[] engineDigest() {
		return digest(param);
	}

	protected int engineDigest(byte[] buf, int off, int len)
			throws DigestException {
		return digest(param, buf, off, len);
	}

	protected int engineGetDigestLength() {
		return 24;
	}

	protected void engineReset() {
		reset(param);
	}

	protected void engineUpdate(byte input) {
		update(param, input);
	}

	protected void engineUpdate(byte[] input, int off, int len) {
		update(param, input, off, len);
	}

	protected void engineUpdate(ByteBuffer input) {
		update(param, input.array(), input.position(), input.remaining());
	}

	protected void finalize() {
		freeParam(param);
	}
}

package il.ac.tau.cs.sw1.musictunes;

/**
 * General Exception for MusicTunes. Every exception instance created by directly
 * MusicTunes code should be of this class or extending classes.
 */
public class MusicTunesException extends Exception {

	private static final long serialVersionUID = -3247403127046962680L;

	public MusicTunesException(String message, Throwable origin) {
		super(message, origin);
	}

	public MusicTunesException(String message) {
		super(message);
	}

}

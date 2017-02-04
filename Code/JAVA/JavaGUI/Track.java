package il.ac.tau.cs.sw1.musictunes;

public class Track {
	
	private String name;
	private int seconds;

	/**
	 * @pre name != null
	 * @pre seconds >= 0
	 * @post name.equals(getName())
	 * @post seconds == getSeconds()
	 */
	public Track(String name, int seconds) {
		this.name=name;
		this.seconds=seconds;
	}

	/**
	 * Returns the name of this track
	 */
	public String getName() {
		return this.name;
	}

	/**
	 * Returns the length of this track in seconds
	 */
	public int getSeconds() {
		return this.seconds;
	}

	@Override
	public String toString() {
		return getName() + " (" + getSeconds() + " secs)";
	}
}

package il.ac.tau.cs.sw1.musictunes;

public class Artist {
	
	private String name;

	/**
	 * @pre name != null
	 * @post name.equals(getName())
	 */
	public Artist(String name) {
		this.name=name;
	}

	/**
	 * Returns the artist name
	 */
	public String getName() {
		return this.name;
	}

	@Override
	public String toString() {
		return getName();
	}
}

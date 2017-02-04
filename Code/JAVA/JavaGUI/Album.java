package il.ac.tau.cs.sw1.musictunes;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;


public class Album {
	
	private String name;
	private int year;
	private Set<Artist> albumArtists=new HashSet<Artist>();
	@SuppressWarnings({ "unchecked", "rawtypes" })
	private List<Track> albumTracks=new ArrayList();

	/**
	 * @pre name != null
	 * @post name.equals(getName())
	 * @post year == getYear()
	 */
	public Album(String name, int year) {
		this.name=name;
		this.year=year;
	}

	/**
	 * Returns the name of this album
	 */
	public String getName() {
		return this.name;
	}

	/**
	 * Returns the year in which this album was released
	 */
	public int getYear() {
		return this.year;
	}

	@Override
	public String toString() {
		return getName() + " (" + getYear() + ")";
	}

	/**
	 * Adds an artist to this album.
	 * 
	 * @pre albumArtist != null
	 */
	public void addArtist(Artist albumArtist) {
		this.albumArtists.add(albumArtist);
	}

	/**
	 * Returns all the artists added to this album (so far) ordered
	 * lexicographically by the artist name
	 */
	public Set<Artist> getAlbumArtists() {
		Comparator<Artist> arNameComparator = new Comparator<Artist>(){
				public int compare(Artist ar1, Artist ar2) {
					int compare=(ar1.getName().compareTo(ar2.getName()));
					return compare;
				}
					
		};
		SortedSet<Artist> sortedArtists=new TreeSet<Artist>(arNameComparator);
		sortedArtists.addAll(this.albumArtists);
		return sortedArtists;
	}
	
	
	/**
	 * Adds a track to this album.
	 * 
	 * @pre track != null
	 */
	public void addTrack(Track track) {
		this.albumTracks.add(track);
	}

	/**
	 * Returns all the tracks added to this album (so far) by order of addition
	 * to the album
	 */
	public List<Track> getTracks() {
		return this.albumTracks;
	}
	
	public void setName(String name){
		this.name=name;
	}
	
	public void setYear(int year){
		this.year=year;
	}
}

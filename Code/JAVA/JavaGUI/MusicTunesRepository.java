package il.ac.tau.cs.sw1.musictunes;

import java.util.Comparator;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;

public class MusicTunesRepository {
	
	private Set<Album> albums=new HashSet<Album>();
	
	private int count=0;

	/**
	 * Adds an album for this repository. If the repository already contains an
	 * album of the same name and year, nothing happens
	 * 
	 * @pre album != null
	 */
	public void addAlbum(Album album) {
		albums.add(album);
		count++;
	}
	
	/**
	 * Returns the albums added to this repository (so far) ordered by their
	 * names and then by their release years
	 */
	public Set<Album> getAlbums() {
		Comparator<Album> alComparator = new Comparator<Album>(){
			public int compare(Album al1, Album al2) {
				int compare=(al1.getName().compareTo(al2.getName()));
				if (compare==0)
					compare=(Integer.compare(al1.getYear(),al2.getYear()));
				return compare;
			}
				
	};
	SortedSet<Album> sortedAlbums=new TreeSet<Album>(alComparator);
	sortedAlbums.addAll(this.albums);
	return  sortedAlbums;
	}
	
	public void getAlbumsNames(){
		Iterator<Album> it = this.getAlbums().iterator(); 
		while (it.hasNext()){
			Album currentAlbum=it.next();
			System.out.println(currentAlbum.getName());
		}
	}
	
	public int getSize(){
		return this.count;
	}

}

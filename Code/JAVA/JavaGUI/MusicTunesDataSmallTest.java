package il.ac.tau.cs.sw1.musictunes;

public class MusicTunesDataSmallTest {

	public static void main(String[] args) {
		MusicTunesRepository rep = new MusicTunesRepository();
		Artist datz = new Artist("Datz");
		Artist datza = new Artist("Datza");
		Artist shoshana = new Artist("Shoshana");

		Album theBest = new Album("The best", 1991);
		theBest.addArtist(datza);
		theBest.addArtist(datz);
		theBest.addTrack(new Track("Kan", 193));
		theBest.addTrack(new Track("Interlude", 233));
		rep.addAlbum(theBest);

		Album theBest2011 = new Album("The best of the best", 2011);
		rep.addAlbum(theBest2011);

		Album theBest2013 = new Album("The best", 2013);
		theBest2013.addArtist(datz);
		theBest2013.addArtist(shoshana);
		theBest2013.addTrack(new Track("Kan", 194));
		rep.addAlbum(theBest2013);
		rep.addAlbum(theBest2013);

		String albumsAsString = String.valueOf(rep.getAlbums());
		testEquals("Albums order", albumsAsString,
				"[The best (1991), The best (2013), The best of the best (2011)]");

		String artistsOrder = String.valueOf(theBest
				.getAlbumArtists());
		testEquals("Artists order (in " + theBest + ")",
				artistsOrder, "[Datz, Datza]");

		String tracksOrder = String.valueOf(theBest.getTracks());
		testEquals("Tracks order (in " + theBest + ")", tracksOrder,
				"[Kan (193 secs), Interlude (233 secs)]");
	}

	private static void testEquals(String title, String result,
			String expected) {
		System.out.println("#### " + title);
		System.out.println("Got:\t\t" + result);
		System.out.println("Expected:\t" + expected);
		System.out.println(expected.equals(result) ? "Success!"
				: "Failure...");
		System.out.println();
	}

}

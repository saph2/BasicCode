package il.ac.tau.cs.sw1.musictunes;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.*;

public class GraphicalMusicTunesViewer {

	private static final String APPLICATION_CREATION_DATE = "12/01/2014";
	private static final String YOUR_NAME = "Sapir Natan";
		
	public static void main(String[] args) {
		showViewer();
	}

	public static void showViewer() {
		GraphicalMusicTunesViewer viewer = new GraphicalMusicTunesViewer();
		viewer.show();
	}

	private Shell shell; // the main application window
	private Label albumStatus; // the status label for albums
	private Tree albumsTree; // the tree in the albums tab
	private Label artistsStatus; // the artists status label
	private Tree artistsTree; // the tree in the artists tab
	
	public static MusicTunesRepository allAlbums = new MusicTunesRepository();//will contain all albums
	
	private Map<String,Set<String>> artistsAlbums=new HashMap<String,Set<String>>();//will contain all artists and their albums

	private GraphicalMusicTunesViewer() {
		// constructor is private, instances can only be created from this class
	}

	private void show() {
		createShell();
		runApplication();
	}

	/**
	 * Constructs the main window of the application
	 */
	private void createShell() {
		Display display = Display.getDefault();
		shell = new Shell(display);
		shell.setText(GUIUtils.MUSIC_TUNES);

		// window style
		Rectangle monitor_bounds = shell.getMonitor().getBounds();
		shell.setSize(new Point(monitor_bounds.width / 3,
				monitor_bounds.height / 2));
		shell.setLayout(new FillLayout());

		// create the different components within the main window
		createMenu();
		createTabs();
	}

	/**
	 * Creates the menu bar of the main window
	 */
	private void createMenu() {
		Menu menu = new Menu(shell, SWT.BAR);
		shell.setMenuBar(menu);

		createAboutMenu(menu);
	}

	/**
	 * Constructs the "about" button in the menu bar
	 */
	private void createAboutMenu(Menu menu) {
		MenuItem aboutItem = new MenuItem(menu, SWT.PUSH);
		aboutItem.setText("About");
		aboutItem.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				showAbout();
				}
		});
	}

	/**
	 * Opens a pop-up displaying the details about the program
	 */
	private void showAbout() {
		GUIUtils.showInfoDialog(shell, "About MusicTunes",
				"MusicTunes was created by " + YOUR_NAME + " on "
						+ APPLICATION_CREATION_DATE
						+ "\nAll rights reserved");
	}

	/**
	 * Create the tabs in the main window - the albums tab and the artists tab
	 */
	private void createTabs() {
		TabFolder tabFolder = new TabFolder(shell, SWT.BORDER);

		createViewAlbumsTab(tabFolder);
		createViewArtistsTab(tabFolder);

		tabFolder.pack();
	}

	/**
	 * Creates the albums tab
	 * 
	 * @param tabFolder
	 */
	private void createViewAlbumsTab(TabFolder tabFolder) {
		// the tab itself
		TabItem albumsTab = new TabItem(tabFolder, SWT.NONE);
		albumsTab.setText("Albums");

		// the main container in the tab
		Composite albumsContent = new Composite(tabFolder, SWT.None);
		albumsContent.setLayout(new GridLayout(2, false));
		albumsTab.setControl(albumsContent);

		// the status line in the tab
		albumStatus = new Label(albumsContent, SWT.NONE);
		albumStatus.setLayoutData(new GridData(
				GridData.FILL_HORIZONTAL));
		albumStatus.setText("No albums found.");

		// the "Add Album" button
		Button newAlbumButton = new Button(albumsContent, SWT.PUSH);
		newAlbumButton.setText("Add Album");
		newAlbumButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				addAlbum();
				}
		});
		// the tree with albums (initially empty)
		albumsTree = new Tree(albumsContent, SWT.SINGLE | SWT.BORDER);
		GridData treeLayoutData = new GridData(GridData.FILL_BOTH);
		treeLayoutData.horizontalSpan = 2;
		albumsTree.setLayoutData(treeLayoutData);
	}

	/**
	 * Opens a dialog where the user can enter all the details of an album
	 * (including artists and tracks) The new album will be added to the repository
	 * albums and displayed in the UI.
	 */
	private void addAlbum() {	
		albumsTree.removeAll();//Initialize albums tree
		artistsTree.removeAll();//Initialize artists tree
		
		CreateAlbumDialog album = new CreateAlbumDialog(shell);//open dialog
		album.open(allAlbums);
		
		Iterator<Album> it = allAlbums.getAlbums().iterator(); 
		
		while (it.hasNext()){//creates the albums tree and inside the artists tree
			Album newAlbum=it.next();
			createTrees(newAlbum);
		}
		albumStatus.setText("Number of albums: "+allAlbums.getSize());//updates album count
		artistsStatus.setText("Number of artists: "+this.artistsAlbums.size());//updates artists count
	}
	
	private void createTrees(Album newAlbum){
		//creates the tree for each album and updates the artists tree		
		
		TreeItem albumItem=new TreeItem(albumsTree,0);//new album label
		albumItem.setText(newAlbum.getName()+" ("+newAlbum.getYear()+")");
		
		Set<Artist> artists=newAlbum.getAlbumArtists();//artists label and info
		TreeItem albumArtists=new TreeItem(albumItem,0);
		albumArtists.setText("Album Artists");
		TreeItem items[]=artistsTree.getItems();
			
		Iterator<Artist> it = artists.iterator();
		
		while (it.hasNext()){//creates the artists tree
				
			String atristName=it.next().getName();
			TreeItem artist=new TreeItem(albumArtists,0);
			artist.setText(atristName);
			
			// creating artists tree
						
				boolean exists=false;
							
				for (TreeItem i: items){//go over the tree
						if (i.getText().toString().equals(atristName)){//checks if the artists is in the tree
								if (!artistsAlbums.get(atristName).contains(newAlbum.getName())){//if this is a new album for the artist
									artistsAlbums.get(atristName).add(newAlbum.getName());//update artists list
									TreeItem artistsAlbum=new TreeItem(i,0);//add album to this artist
									artistsAlbum.setText(newAlbum.getName()+" ("+newAlbum.getYear()+")");
									exists=true;
									break;
								}
							}
						}
				
				if (exists==false){//if new artist
					Set<String> albums=new HashSet<String>();
					albums.add(newAlbum.getName());
					artistsAlbums.put(atristName, albums);//update artists list
					TreeItem artistItem=new TreeItem(artistsTree,0);//add new artist and album to the tree
					artistItem.setText(atristName);
					TreeItem artistsAlbum=new TreeItem(artistItem,0);
					artistsAlbum.setText(newAlbum.getName()+" ("+newAlbum.getYear()+")");
				}
			}
		
		
		TreeItem albumTracks=new TreeItem(albumItem,0);//track's label and info
		albumTracks.setText("Tracks");
		ArrayList<Track> tracks=(ArrayList<Track>) newAlbum.getTracks();
		for (int i=0; i<tracks.size(); i++){//add tracks to album
			TreeItem currentTrack=new TreeItem(albumTracks,0);
			currentTrack.setText(tracks.get(i).getName()+" ("+tracks.get(i).getSeconds()+"secs)");
		}
		
	}
	
	/**
	 * Creates the tab with artists
	 * 
	 * @param tabFolder
	 */
	private void createViewArtistsTab(TabFolder tabFolder) {
		// the tab itself
		TabItem albumsTab = new TabItem(tabFolder, SWT.NONE);
		albumsTab.setText("Artists");

		// The tab main container
		Composite artistsContent = new Composite(tabFolder, SWT.None);
		artistsContent.setLayout(new GridLayout());
		albumsTab.setControl(artistsContent);

		// The artists status line
		artistsStatus = new Label(artistsContent, SWT.NONE);
		artistsStatus.setLayoutData(new GridData(
				GridData.FILL_HORIZONTAL));
		artistsStatus.setText("No artists found.");

		// The tree with artists and their albums (initially empty)
		artistsTree = new Tree(artistsContent, SWT.SINGLE
				| SWT.BORDER);
		artistsTree.setLayoutData(new GridData(GridData.FILL_BOTH));
	}

	/**
	 * Opens the application main window, and runs the "event loop" in the
	 * standard SWT way.
	 */
	private void runApplication() {
		shell.open();
		Display display = shell.getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
		display.dispose();
	}
}

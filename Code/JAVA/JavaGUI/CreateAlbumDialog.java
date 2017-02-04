package il.ac.tau.cs.sw1.musictunes;


import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;



/**
 * A dialog that allows the entering the details of an album (including artists
 * and tracks)
 */
public class CreateAlbumDialog extends MusicTunesDialog {

	private static final String TRACK_NAME = "Track name";
	private static final String YEAR = "Year:";
	private static final String NAME = "Name:";
	private static final String TRACK_LENGTH = "Track length";
	
	private Text nameField; // album name field
	private Text yearField; // album release year field
	private Text artistNameField; // artist name to add field
	private Text trackNameField; // track name to add field
	private Text trackLengthField; // track length to add field
	private List artistList; // list of artists (org.eclipse.swt.widgets.List)
	private List trackList; // list of tracks (org.eclipse.swt.widgets.List)
	
	public Album newAlbum=new Album("NoName", 0);//the new album opened with the window to contain the data
	
	public boolean okToCreate=false;//returns if the album is legal and can be added to the album repository
	
	public GUIUtils error;//handles exceptions

	/**
	 * Creates a new dialog (but does not open it yet. To open the dialog call
	 * open().)
	 * 
	 * @param parent
	 *            - the window from which this dialog was opened
	 */
	public CreateAlbumDialog(Shell parent) {
		super(parent, "Add Album");
	}	
	
	public void open(MusicTunesRepository allAlbums){
		super.open();
		if (this.okToCreate)//when an "ok" is received the new album will be added to the albums repository
			allAlbums.addAlbum(this.newAlbum);
			
	}

	/**
	 * Creates the part of the dialog with the form and fields to fill in
	 */
	@Override
	protected void createForm() {
		// create the form
		Composite form = new Composite(dialogShell, SWT.BORDER);
		form.setLayout(new GridLayout());

		createAlbumDetailsFields(form);

		createArtistsDetailsFields(form);

		createTracksDetailsFields(form);

		createOKAndCancel(form);
	}

	/**
	 * Creates the track details group of fields
	 */
	private void createTracksDetailsFields(Composite form) {
		Group tracksDetails = createGroup(form, "Album tracks", 3);

		// creates the list of tracks (in the UI)
		trackList = new List(tracksDetails, SWT.BORDER | SWT.V_SCROLL
				| SWT.H_SCROLL);
		GridData trackListLayout = new GridData(GridData.FILL_BOTH);
		trackListLayout.horizontalSpan = 3;
		trackList.setLayoutData(trackListLayout);

		// creates the track name field
		trackNameField = createFieldWithLabel(tracksDetails, NAME, 2);
		// creates the track length field
		trackLengthField = createFieldWithLabel(tracksDetails,
				"Length (seconds):", 1);

		// when this button in pressed, a track is added to the track list
		createButton(tracksDetails, "Add Track",
				new SelectionAdapter() {
				public void widgetSelected(SelectionEvent e) {
					addTrackToAlbum();
				}
				});
	}

	/**
	 * Creates the artists details group of fields
	 */
	private void createArtistsDetailsFields(Composite form) {
		Group artistsDetails = createGroup(form, "Album artists", 3);

		// creates the list of artists (in the UI)
		artistList = new List(artistsDetails, SWT.BORDER
				| SWT.V_SCROLL | SWT.H_SCROLL);
		GridData artistListLayout = new GridData(GridData.FILL_BOTH);
		artistListLayout.horizontalSpan = 3;
		artistList.setLayoutData(artistListLayout);

		// creates the artist name field
		artistNameField = createFieldWithLabel(artistsDetails, NAME,
				1);

		// when this button in pressed, an artist is added to the artists list
		createButton(artistsDetails, "Add Artist",
				new SelectionAdapter() {
				public void widgetSelected(SelectionEvent e) {
				addArtistToAlbum();
				}
				});
	}

	/**
	 * Creates the album details group of fields
	 */
	private void createAlbumDetailsFields(Composite form) {
		Group albumDetails = createGroup(form, "Album details", 2);

		nameField = createFieldWithLabel(albumDetails, NAME, 1);
		yearField = createFieldWithLabel(albumDetails, YEAR, 1);
		
	}

	/**
	 * Creates a group of fields with a title and 3 columns in the layout
	 */
	private Group createGroup(Composite form, String title,
			int numColumns) {
		Group group = new Group(form, SWT.NULL);
		group.setText(title);
		group.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		group.setLayout(new GridLayout(numColumns, false));
		return group;
	}

	/**
	 * Adds the artist to the list of album artists, and also to the list in the
	 * UI (unless such artist already exists)
	 * 
	 * hint: use org.eclipse.swt.widgets.List.add(String string)
	 */
	@SuppressWarnings("static-access")
	private void addArtistToAlbum() {
		String artistName=artistNameField.getText().toString();
		if (!artistName.equals(null)){
			try {//if the artist's name is legal it will be added to the album's artists list
				error.verifyNotEmpty(artistNameField,NAME);
				artistList.add(artistName);//add to track list
				newAlbum.addArtist(new Artist(artistName));
			} 
			catch (MusicTunesException e) {
				error.showErrorDialog(dialogShell, e);
			}
		}
		else
			return;
	}

	/**
	 * Adds the track to the album's list of tracks, and also to the list in the
	 * UI
	 * 
	 * hint: use org.eclipse.swt.widgets.List.add(String string)
	 */
	@SuppressWarnings("static-access")
	private void addTrackToAlbum() {
		String trackName=trackNameField.getText().toString();
		String trackSec=trackLengthField.getText().toString();
		try{//if the track is legal it will be added to the album's tracks list
				error.verifyNotEmpty(trackNameField, TRACK_NAME);
				error.verifyNotEmpty(trackLengthField, TRACK_LENGTH);
				error.verifyInt(trackLengthField, TRACK_LENGTH);
				error.verifyNonNegative(Double.valueOf(trackLengthField.getText()), TRACK_LENGTH);
				trackList.add(trackName+" ("+trackSec+"secs)");//to be checked
				newAlbum.addTrack(new Track(trackName, Integer.parseInt(trackSec)));
			}
			catch (MusicTunesException e){
				error.showErrorDialog(dialogShell, e);
			}
		}

	@SuppressWarnings("static-access")
	@Override
	protected void setActionFailed(){
		try{//if album's data (name + year) is legal an ok is send to add the album to the repository and close dialog
			error.verifyNotEmpty(nameField, NAME);
			error.verifyNotEmpty(yearField,YEAR);
			error.verifyInt(yearField, YEAR);
			error.verifyNonNegative(Double.valueOf(yearField.getText()),YEAR);
			this.newAlbum.setName(nameField.getText().toString());
			this.newAlbum.setYear(Integer.parseInt(yearField.getText().toString()));
			this.okToCreate=true;
		}
		catch (MusicTunesException x){
			error.showErrorDialog(dialogShell, x);
		}
	}

	/**
	 * Creates the listener to be activated when the "ok" button is pressed.
	 * Verifies the validity of the values in the different fields, and if they
	 * are valid a new album is created, and the dialog is closed (hint: use
	 * org.eclipse.swt.widgets.Shell.dispose()). Otherwise an error message is
	 * displayed.
	 */
	@Override
	protected SelectionListener getOkSelectionListener() {
		return (new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
					setActionFailed();//check album's legality
					if (okToCreate==true)//when the album is legal the dialog will be closed (or canceled)
						dialogShell.dispose();
		}
			
			}); 
	}
	
	@Override
	protected SelectionListener getCancelSelectionListener() {
		return new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				dialogShell.dispose();
			}

		};
	}

}

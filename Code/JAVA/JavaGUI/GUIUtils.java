package il.ac.tau.cs.sw1.musictunes;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

public class GUIUtils {

	public static final String NO_ALBUMS = "No albums found.";
	public static final String NO_ARTISTS = "No artists found.";
	public static final String NO_TRACKS = "No tracks found.";
	public static final String MUSIC_TUNES = "MusicTunes";

	/**
	 * generates the message to display to the user. AddressBookException
	 * messages are trusted, other exceptions - not.
	 * 
	 * @param shell
	 * @param e
	 *            the error that occurred
	 */
	public static void showErrorDialog(Shell shell, Exception e) {
		String message = "Oops... an unexpected error occurred";
		if (e != null) {
			if (e instanceof MusicTunesException) {
				message = e.getMessage();
			}
		}
		showErrorDialog(shell, message);
	}

	/**
	 * open a dialog with an error message to the user.
	 * 
	 * @param shell
	 * @param message
	 *            to display
	 */
	public static void showErrorDialog(Shell shell, String message) {
		showDialog(shell, "Error", message, SWT.ICON_ERROR);
	}

	/**
	 * open a dialog with information to the user.
	 * 
	 * @param shell
	 * @param title
	 *            of the dialog
	 * @param message
	 *            to display
	 */
	public static void showInfoDialog(Shell shell, String title,
			String message) {
		showDialog(shell, title, message, SWT.ICON_INFORMATION);
	}

	/**
	 * Opens a message dialog
	 * 
	 * @param shell
	 *            - the parent window
	 * @param title
	 *            - the dialog title
	 * @param message
	 *            - the message to display
	 * @param icon
	 *            - the type of SWT icon to display
	 */
	private static void showDialog(Shell shell, String title,
			String message, int icon) {
		MessageBox messageBox = new MessageBox(shell, icon | SWT.OK);
		messageBox.setText(title);
		messageBox.setMessage(message);
		messageBox.open();
	}

	/***********************************************
	 * Field verification methods
	 */

	/**
	 * Verifies that the field contains a non-empty string
	 * 
	 * @param field
	 *            - the text field
	 * @param fieldName
	 *            - the field name (for display purposes)
	 * @throws MusicTunesException
	 *             - if the field is empty
	 */
	protected static void verifyNotEmpty(Text field, String fieldName)
			throws MusicTunesException {
		if (field.getText().length() == 0) {
			field.setFocus();
			field.selectAll();
			throw new MusicTunesException(fieldName
					+ " field must be filled!");
		}
	}

	/**
	 * Verifies that the field contains a string that can be interpreted as a
	 * long value
	 * 
	 * @param field
	 *            - the text field
	 * @param fieldName
	 *            - the field name (for display purposes)
	 * @return - the parsed number, if succeeded
	 * @throws MusicTunesException
	 *             - if the field content cannot be interpreted as a long value
	 */
	protected static long verifyLong(Text field, String fieldName)
			throws MusicTunesException {
		long number = 0;
		try {
			number = Long.parseLong(field.getText());
		} catch (NumberFormatException e2) {
			field.setFocus();
			field.selectAll();
			throw new MusicTunesException("Invalid " + fieldName
					+ ": " + field.getText());
		}
		return number;
	}

	/**
	 * Verifies that the field contains a string that can be interpreted as an
	 * int value
	 * 
	 * @param field
	 *            - the text field
	 * @param fieldName
	 *            - the field name (for display purposes)
	 * @return - the parsed int, if succeeded
	 * @throws MusicTunesException
	 *             - if the field content cannot be interpreted as an int value
	 */
	protected static int verifyInt(Text field, String fieldName)
			throws MusicTunesException {
		int number = 0;
		try {
			number = Integer.parseInt(field.getText());
		} catch (NumberFormatException e2) {
			field.setFocus();
			field.selectAll();
			throw new MusicTunesException("Invalid " + fieldName
					+ ": " + field.getText());
		}
		return number;
	}

	/**
	 * Verifies that the a certain value, originating at a field, is
	 * non-negative
	 * 
	 * @param number
	 *            - the number to verify
	 * @param fieldName
	 *            - the field name (for display purposes)
	 * @throws MusicTunesException
	 *             - if number is negative
	 */
	protected static void verifyNonNegative(double number,
			String fieldName) throws MusicTunesException {
		if (number < 0) {
			throw new MusicTunesException(fieldName
					+ " must be non-negative");
		}
	}

}

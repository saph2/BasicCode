package il.ac.tau.cs.sw1.musictunes;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

/**
 * A general dialog window that may contain a form with some fields, buttons,
 * etc.
 */
public abstract class MusicTunesDialog extends Dialog {

	protected Shell dialogShell;
	private String title;

	public MusicTunesDialog(Shell parent, String title) {
		super(parent);
		this.title = title;
	}

	/**
	 * Should be called before the dialog is closed if the action failed
	 */
	protected abstract void setActionFailed();

	/**
	 * @return a selection listener that should be activated when pressing the
	 *         "ok" button
	 */
	protected abstract SelectionListener getOkSelectionListener();

	/**
	 * opens the dialog
	 * 
	 * @return null if cancel was pressed, a contact otherwise
	 */
	public void open() {
		createDialogShell();
		createForm();
		showDialog();
	}

	/**
	 * Creates the dialog window
	 */
	private void createDialogShell() {
		dialogShell = new Shell(getParent(), SWT.DIALOG_TRIM
				| SWT.APPLICATION_MODAL);
		dialogShell.setText(title);
		dialogShell.setLayout(new FillLayout(SWT.VERTICAL));
	}

	/**
	 * To be implemented by inheriting classes - the form displayed by this
	 * dialog
	 */
	protected abstract void createForm();

	/**
	 * Opens the dialog window and runs the "event loop" in the stndard SWT way.
	 */
	private void showDialog() {
		// resize the window
		Rectangle monitor_bounds = dialogShell.getMonitor()
				.getBounds();
		dialogShell.pack();
		dialogShell.setSize(monitor_bounds.width / 3,
				dialogShell.getSize().y);

		// open the dialog
		dialogShell.open();

		// event loop
		Display display = getParent().getDisplay();
		while (!dialogShell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
	}

	/**
	 * A utility function - create a button
	 * 
	 * @param form
	 *            - the form container
	 * @param text
	 *            - the text on the button
	 * @param listener
	 *            - the listener to the button selection action
	 * @return the button
	 */
	protected Button createButton(Composite form, String text,
			SelectionListener listener) {
		Button button = new Button(form, SWT.PUSH);
		button.setText(text);
		button.addSelectionListener(listener);
		return button;
	}

	/**
	 * A utility function - create a listener for the dialog "Cancel" button.
	 * This button only calls setActionFailed() and closes the dialog.
	 * 
	 * @return the listener
	 */
	protected SelectionListener getCancelSelectionListener() {
		return new SelectionAdapter() {
			/**
			 * @see org.eclipse.swt.events.SelectionAdapter#widgetSelected(org.eclipse.swt.events.SelectionEvent)
			 */
			@Override
			public void widgetSelected(SelectionEvent e) {
				setActionFailed();
				dialogShell.dispose();
			}

		};
	}

	/**
	 * A utility function: creates a field with a name and label
	 * 
	 * @param form
	 *            - the form container
	 * @param labelText
	 *            - the field label
	 * @param fieldSpan
	 *            - the number of columns the text field occupies (assuming it
	 *            is in GridLayout)
	 * @return the text field
	 */
	protected Text createFieldWithLabel(Composite form,
			String labelText, int fieldSpan) {
		// create the label
		Label label = new Label(form, SWT.NONE);
		label.setText(labelText);
		label.setLayoutData(new GridData(GridData.BEGINNING,
				GridData.CENTER, false, false));

		// create the text field
		Text field = new Text(form, SWT.SINGLE | SWT.BORDER);
		GridData nameLayoutData = new GridData(GridData.FILL,
				GridData.CENTER, true, false);
		nameLayoutData.horizontalSpan = fieldSpan;
		field.setLayoutData(nameLayoutData);
		return field;
	}

	/**
	 * Creates the OK and Cancel buttons of the dialog
	 * 
	 * @return the button container
	 */
	protected Composite createOKAndCancel(Composite form) {
		Composite buttonGroup = new Composite(form, SWT.NONE);
		buttonGroup.setLayout(new FillLayout());

		createButton(buttonGroup, "Ok", getOkSelectionListener());
		createButton(buttonGroup, "Cancel",
				getCancelSelectionListener());
		return buttonGroup;
	}
	

}
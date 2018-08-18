import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
from tkinter import *

background_frame_color = "MistyRose"
background_button_color = "RosyBrown1"
background_tab_color = "RosyBrown2"
frame_font = "Ariel 10 bold italic"
tab_font = "Ariel 12 bold"
button_font = "Ariel 10 bold"
selection_font = "Ariel 10 bold"

class GUI_Win():

    height = 12
    width = 12

    def __init__(self, master, selected = []):
        self.master = master
        self.selected = selected
        self.win_frame = None
        self.add_frame()
    
    def get_selected(self):
        return list(self.selected)

    def clear_selected(self):
        self.selected.clear()

    def select(self):
        return
    
    def add_frame(self):
        return

    def get_frame(self):
        return self.win_frame

class GUI_Win_Scrollbar(GUI_Win):
    
    def __init__(self, master, selected = []):
        super().__init__(master, selected=set())

    def select(self, args):
        self.clear_selected()
        self.selected.add(self.var.get())
       # print (self.selected)

    def add_frame(self):
        self.var = StringVar(self.master)
        self.var.set("Disable")
        self.win_frame = LabelFrame(self.master, text="Option", font=frame_font, padx=5, pady=5, background=background_frame_color)
        option_menu = OptionMenu(self.win_frame, self.var, "Disable", "Enable", command=self.select)
        option_menu.config(bg=background_frame_color)
        option_menu.config(font=selection_font)
        option_menu.pack(anchor="w")
        self.win_frame.pack(anchor="w")

class GUI_Win_Checkbox(GUI_Win):
        
    def __init__(self, master, selected = []):
        super().__init__(master, selected=set())
        
    def state(self):
        return map((lambda var: var.get()), self.vars)
    
    def select(self):
        self.clear_selected()
        for value in (list(self.state())):
            if (value != '0'):
                self.selected.add(value)
        #print (self.selected)

    def add_checkbutton(self, option):
        var = StringVar()
        cb = Checkbutton(self.win_frame, text=option, font=selection_font, variable=var, onvalue = option, offvalue = None, command=self.select, background=background_frame_color)
        cb.pack(anchor="w")
        self.vars.append(var)

    def add_frame(self):
        self.vars=[]
        self.win_frame = LabelFrame(self.master, text="Option", font=frame_font, padx=5, pady=5,background=background_frame_color)
        self.add_checkbutton(option="op1")
        self.add_checkbutton(option="op2")
        self.win_frame.pack(anchor="w")


class GUI_Win_Files(GUI_Win):
    
    def __init__(self, master, selected = []):
        super().__init__(master, selected=set())

    def select(self):
        self.lstbox.pack_forget()
        file_path = filedialog.askopenfilename()
        self.selected.add(file_path)
        self.var.set(list(self.selected))
        self.lstbox.pack(anchor="w",fill="both", expand=True)

    def add_frame(self):
        self.win_frame = LabelFrame(self.master, text="Files", font=frame_font, padx=5, pady=5, background=background_frame_color)
        self.var = StringVar(self.win_frame)
        self.btk_add = Button(master=self.win_frame, text="Add", font=button_font, command=self.select, background=background_button_color)
        self.btk_add.pack(anchor="w")
        self.lstbox = Listbox(self.win_frame, listvariable=self.var, selectmode=MULTIPLE, width=60, height=12)
        self.lstbox.pack(anchor="w",fill="both", expand=True)
        self.win_frame.pack(anchor="w",fill="both", expand=True)

class GUI_WIN_Tab():
    
    def __init__(self, master, name):
        self.master=master
        self.name = name
        self.add_frame()

    def set_tab(self):
        self.scroll_win = GUI_Win_Scrollbar(master=self.win_frame)
        self.check_win = GUI_Win_Checkbox(master=self.win_frame)
        self.files_win = GUI_Win_Files(master=self.win_frame)
    
    def add_frame(self):
        self.win_frame = LabelFrame(master=self.master, background=background_frame_color)
        self.win_frame.pack(anchor="w",fill="both", expand=True)
        self.set_tab()

    def get_frame(self):
        return self.win_frame

    def get_name(self):
        return self.name
        
class GUI_Win_Tabs():
     
    def __init__(self, master):
        self.master=master
        self.all_tabs = list()
        self.add_frame()

    def add_tab(self, new_tab):
        self.notebook.add(new_tab.get_frame(), text=new_tab.get_name())
        self.all_tabs.append(new_tab)

    def get_tabs(self):
        return self.all_tabs

    def get_selected(self):
        return map((lambda tab: tab.get_selected()), self.all_tabs)

    def style_tabs(self):
        self.style = ttk.Style()
        self.style.theme_create( "tabs", parent="alt", settings={
        "TNotebook": {"configure": {"background": background_frame_color}},
        "TNotebook.Tab": {
            "configure": {"background": background_frame_color, "font": tab_font},
            "map":       {"background": [("selected", background_tab_color)] } } } )
        
        self.style.theme_use("tabs")
        self.notebook = ttk.Notebook(self.win_frame)

    def add_frame(self):
        self.win_frame = LabelFrame(master=self.master, text="Option", font=frame_font, background=background_frame_color)
        self.style_tabs()
        self.notebook.pack(anchor="w", fill="both", expand=True)
        self.win_frame.pack(anchor="w",fill="both", expand=True)
    
    def get_frame(self):
        return self.win_frame


class GUI_WIN_Main():
    
    def __init__(self, master):
        self.master=master
        self.add_frame()

    def run(self):
        print ("Running")
    
    def add_tabs(self):
        self.tabs_win = GUI_Win_Tabs(self.win_frame)
        self.tabs_frame = self.tabs_win.get_frame()
        new_tab = GUI_WIN_Tab(self.tabs_frame, name="tab1")
        self.tabs_win.add_tab(new_tab)
        new_tab = GUI_WIN_Tab(self.tabs_frame, name="tab2")
        self.tabs_win.add_tab(new_tab)

    def add_manu_options(self):
        self.menu_frame = LabelFrame(self.win_frame, text="Menu", font=frame_font, padx=5, pady=5, background=background_frame_color)
        self.run_btn = Button(self.menu_frame, text="Run", font=button_font, command=self.run, background=background_button_color)
        self.run_btn.pack(anchor="w")
        self.menu_frame.pack(anchor="w")

    def add_frame(self):
        self.win_frame = Frame(self.master, padx=5, pady=5, background=background_frame_color)
        self.add_manu_options()
        self.add_tabs()
        self.win_frame.pack(anchor="w",fill="both", expand=True)


class GUI_Display():
    
    def __init__(self):
        self.master = tk.Tk()
        self.master.title("Title")
        self.master.configure(background=background_frame_color)
        self.frames = dict()

    def get_display(self):
        return self.master

    def open_win(self):
        self.master.mainloop()


gui_display = GUI_Display()
root_display = gui_display.get_display()
main_win = GUI_WIN_Main(root_display)
gui_display.open_win()







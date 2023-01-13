from tkinter import *
from tkinter import ttk
import connect


def send_message ():
    connect.ser.write(b"1")
    connect.ser.write(b"m")


root = Tk ()
root.title ("")
root.geometry ("480x320")

entry = ttk.Entry ()
entry.pack (anchor=NW, padx=6, pady=6)

btn = ttk.Button (text="Drink", command=send_message)
btn.pack (anchor=NW, padx=6, pady=6)





root.mainloop ()

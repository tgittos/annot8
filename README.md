annot8
======

Annot8 is a code annotation tool that will allow you to write notes about code without modifying it.

How it works
------------

How I imagine it working is as a database of notes crossed between an IDE, in a way.
You point annot8 at a directory of code, and you will have a sidebar to navigate the code, and an editor to open the
code into. The editor will be a split pane, on the left the code, and on the right the note window.

You should be able to select a line of code in the left pane, and write notes in the right pane. The note will be
associated with that specific line of code, and the right side will scroll with the left side. It will use graphical
magic (TM) to allow you to write any size of note for a single line, if required, which will compress down as it
scrolls with the code side.

Ideally you will be able to attach images, links, anything you want to your note for that line of code.

As you browse the source and write notes, annot8 will save your notes in a database. You can move between files and
write notes seemlessly.

Annot8 will save your notes in a db file that only has to live in the directory you're annotating. If you move the code,
your notes will move with the project. You can also optionally include the db into source control, and leverage annot8
as a means of documenting code.

Example
-------

Following is an example of the graphica magic (TM) mentioned above:
<img src="https://dl.dropboxusercontent.com/u/30857/annot8-idea.jpg" />

Installation
------------

As of now, this project is still being prototyped. I have no idea if it's even possible, nor how to go about doing it.
Therefore, I will include no instructions on how to get it up and running. Once it starts to take shape and do what I
want it to, I will probably release notes on how to get it up and running.

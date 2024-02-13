# COL331 Lab 1 (Part-A): Primitive Mouse Support in xv6
In ```p5-input```, you have seen how your basic operating system can be modified to handle inputs from an external device - a keyboard.

Modern operating systems also have support for using a mouse or a trackpad. In this lab, you will try to add some primitive support for using the mouse in the basic ```xv6``` OS, which we have been building up from scratch. 

## Introduction

PS/2 is a type of serial communication, typically used for user input devices such as a keyboard or a mouse. A _PS/2 controller_ can be used to communicate both with a PS/2 keyboard and a PS/2 mouse.

A PS/2 controller has two IO ports, which our OS can use to communicate with it:

| I/O PORT | Purpose |
| -------- | --------|
| 0x60     | Data Port |
| 0x64     | Command/Status Port |

A PS/2 mouse typically generates ```IRQ12``` interrupts whenever its state changes. 

## Getting Started 
Interacting with the PS/2 controller is fairly complex and there are several steps involved. Most of your work will be implementing the functions which we have provided you a skeleton for in ```mouse.c```:
```
// Wait until the mouse controller is ready for us to send a packet
void mousewait_send(void);

// Wait until the mouse controller has data for us to receive
void mousewait_recv(void);

// Send a one-byte command to the mouse controller, and wait for it to be properly acknowledged
void mousecmd(uchar cmd);

// To activate the mouse when the system boots up
void mouseinit(void);

// To handle an interrupt raised by the mouse controller 
void mouseintr(void);
```
The first three functions are important to communicate with the controller. You need to ensure that the ```mouseinit``` and ```mouseintr``` functions are called appropriately when needed. There are some constants defined in ```mouse.h``` which may be helpful. 

You are encouraged to go through the following [wiki page](https://wiki.osdev.org/Mouse_Input) on PS/2 mouse input. You may also refer to ```p5-input``` and recall how keyboard interrupts are handled.

## Functions: ```mouse.c```
As a first step, you need to implement the first three functions given above, providing the following functionalities. 

- ```mousewait_send```: Before sending bytes to port 0x60 or 0x64, we must _wait_ for bit 1 (value=2) of port 0x64 to become clear.

- ```mousewait_recv```: Bytes cannot be read from port 0x60, we must _wait_ until bit 0 (value=1) of port 0x64 is set. 

- ```mousecmd(uchar cmd)```: Wait until bytes can be sent to the controller. Send the byte ```0xD4``` to the control port (```0x64```) to tell the PS/2 controller to address the mouse. Then, send the command ```cmd``` to the data port (```0x60```). The controller will then send back an acknowledgement (```0xFA```) - wait till the controller has data for us to receive, and then read the ```ack```.

## Mouse Initialization 
When the system boots, we need to communicate with the PS/2 controller and activate the mouse. We also need to ensure that the controller knows it needs to send interrupts (```IRQ12```) when the mouse state changes. 

To initialize the mouse correctly, you must code up the following procedure in the ```mouseinit``` function:

1. Wait until the controller can receive a control packet.
2. Send ```0xA8``` to the control port, which tells the PS/2 controller to enable the mouse.
3. To tell the controller to send interrupts when the mouse state changes, we need to modify the "Compaq Status Byte", as follows:
    - Wait until the controller is ready for us to send.
    - Send ```0x20``` to the control port, which selects the Compaq Status byte as the data we want to retrieve.
    - Wait until the controller has data for us to receive.
    - Read the status byte from the data port.
    - Set the 2nd bit of the status byte to 1 by ORing it with 0x02. This specifies that interrupts should be enabled.
    - Wait until the controller is ready for us to send.
    - Send ```0x60``` to the control port, which tells the controller we are about to send it the modified status byte.
    - Wait until the controller is ready for us to send.
    - Send the modified status byte to the data port.
4. Use the ```mousecmd``` function to send ```0xF6``` to the controller, which selects "default settings" for the mouse
5. Use the ```mousecmd``` function to send ```0xF4``` to the controller, which tells the mouse to activate and start sending us interrupts.
6. Tell the interrupt controller that we want to receive the mouse interrupt (```IRQ12```) on CPU 0 using ```ioapicenable```. 

Make sure that you call ```mouseinit``` appropriately while booting. For insipiration, see where ```uartinit``` is called. By following these steps, you should be able to initialize the mouse. 

## Mouse Interrupts
The mouse is now hopefully initialized and ready to send interrupts to the OS. We have defined an interrupt number ```IRQ_MOUSE``` as 12 for mouse interrupts in the ```traps.h``` file. You need to ensure you write code to suitably handle the ```IRQ_MOUSE``` interrupt in the xv6 interrupt handler.

In the interrupt handler, you should call the ```mouseintr()``` function, which will implement with the exact logic of the handler. 

Whenever the mouse state changes, the PS/2 controller raises an interrupt. It also has some data to provide the OS, which  sits in the PS/2 controller's buffer until the interrupt handler reads it. Your task is to read the data from the PS/2 controller buffer and process it. For this, here is some information on PS/2 mouse packets.

### Mouse Packets 
A mouse is usually initialized to generate movement packets at a particular rate. The default rate is 100 packets per second if the mouse is being moved. A mouse also generates a packet if a button is either pressed or released.

Mouse Packets are 3 bytes long. The format of the first byte is given in the table below. Note that 'Of' stands for 'overflow'.

| Bit | Meaning |
|---|---|
| 0 | Left button was clicked |
| 1 | Right button was clicked |
| 2 | Middle button was clicked |
| 3 | Always 1 |
| 4 | If 1, mouse was moved in the left direction |
| 5 | If 1, mouse was moved in the down direction |
| 6 | Position of mouse overflowed the screen in X direction |
| 7 | Position of mouse overflowed the screen in Y direction  |

The second byte represents the movement along the X axis, and the third byte the movement along the Y axis. Note that second and third bytes will just be zero if
the mouse was clicked.

We read from the PS/2 controller buffer (from the data port) one byte at a time. To make sense of the data associated with an interrupt, we need to ensure we read 3 bytes to get a full set of data describing the mouse movements. 

Since mouse may be moving rapidly, each movement will get added to the
controller's buffer. In the interrupt handler, we should drain the controller's
buffer.  We can check that the bit number 0 has a value = 0 of the status port
(```0x64```) which means that there is no more data available on the data port
(```0x60```).

<!-- We can check bit number 0 (value = 1) of the status port (```0x64```) to see if data is available on the data port (```0x60```). -->

At this point, you are equipped with the knowledge to do powerful things with the mouse in the future. However, for now, let us limit ourselves to the simple following task using ```cprintf```:

- If there is a left click, print "```LEFT\n```".
- If there is a right click, print "```RIGHT\n```".
- If there is a middle click, print "```MID\n```".

We thus create a mechanism to detect and handle mouse clicks. Convince yourself that using the other data available from the mouse controller, it would not be too difficult to add a more robust and useful mouse functionality to your OS in the future, should you need it.

---

## Deliverables
You will be required to submit the entire ```col331-lab1``` folder with your changes. The code should be able to handle mouse interrupts and print out the required strings on each type of mouse button press. 

In the ```col331-lab1``` root directory, run the following commands:
```
make clean
tar czvf lab1_<entryNumber>.tar.gz *
``` 
This will create a tarball with the name, ```lab1_<entryNumber>.tar.gz``` in the same directory. Submit this tarball on [Moodle](https://moodle.iitd.ac.in/mod/assign/view.php?id=112819). Entry number format: 2020CS10567. (All English letters will be in capitals in the
entry number)

## Auto-grader

Your submission will be evaluated by our autograder which is available [here](https://csciitd-my.sharepoint.com/:u:/g/personal/csy227589_iitd_ac_in/EcT_AvehrEFDorLWN52seDIBtEqrjQy6hH4vm6bu-KGXTw?e=OShK7x). Follow the below instructions to run the autograder locally on your submission:

- Download and unzip the check_scripts.zip.
  ```bash
  unzip check_scripts_lab1.zip
  cd check_scripts
  ```
- Use the following command to run the auto-grader
  ```bash
  bash check.sh <path/to/lab1_<entryNumber>.tar.gz>
  ```
- The given sample script consists of 3 basic test cases. Note that your submission will be evaluated by hidden test cases.


# COL331 Lab 1 (Part-B): Disk I/O request scheduling

## Introduction

This part of the lab uses [`disk.py`](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-disks/disk.py) to familiarize you with how a modern hard drive works. It has a lot of different options, and unlike most of the other simulations, has a graphical animator to show you exactly what happens when the disk is in action. Note that you must have python3 installed to run the simulator file.

Let's do a simple example first. To run the simulator and compute some basic seek, rotation, and transfer times, you first have to give a list of requests to the simulator. This can either be done by specifying the exact requests, or by having the simulator generate some randomly.

We'll start by specifying a list of requests ourselves. Let's do a single request first:

```sh
$ python3 ./disk.py -a 10
```

At this point you'll see:

```
...
REQUESTS [br '10']

For the requests above, compute the seek, rotate, and transfer times.
Use -c or the graphical mode (-G) to see the answers.
```

To be able to compute the seek, rotation, and transfer times for this request, you'll have to know a little more information about the layout of sectors, the starting position of the disk head, and so forth. To see much of this information, run the simulator in graphical mode (-G):

```sh
$ python3 ./disk.py -a 10 -G
```

At this point, a window should appear with our simple disk on it.  The disk head is positioned on the outside track, halfway through sector 6. As you can see, sector 10 (our example sector) is on the same track, about a third of the way around. The direction of rotation is counter-clockwise. To run the simulation, press the "s" key while the simulator window is highlighted.

When the simulation completes, you should be able to see that the disk spent 105 time units in rotation and 30 in transfer in order to access sector 10, with no seek time. Press "q" to close the simulator window.

To calculate this (instead of just running the simulation), you would need to know a few details about the disk. First, the rotational speed is by default set to 1 degree per time unit. Thus, to make a complete revolution, it takes 360 time units. Second, transfer begins and ends at the halfway point between sectors. Thus, to read sector 10, the transfer begins halfway between 9 and 10, and ends halfway between 10 and 11.  Finally, in the default disk, there are 12 sectors per track, meaning that each sector takes up 30 degrees of the rotational space. Thus, to read a sector, it takes 30 time units (given our default speed of rotation).

With this information in hand, you now should be able to compute the seek, rotation, and transfer times for accessing sector 10. Because the head starts on the same track as 10, there is no seek time. Because the disk rotates at 1 degree / time unit, it takes 105 time units to get to the beginning of sector 10, halfway between 9 and 10 (note that it is exactly 90 degrees to the middle of sector 9, and another 15 to the halfway point). Finally, to transfer the sector takes 30 time units.

Now let's do a slightly more complex example:

```sh
$ python3 ./disk.py -a 10,11 -G
```

In this case, we're transferring two sectors, 10 and 11. How long will it take? Try guessing before running the simulation!

As you probably guessed, this simulation takes just 30 time units longer, to transfer the next sector 11. Thus, the seek and rotate times remain the same, but the transfer time for the requests is doubled. You can in fact see these sums across the top of the simulator window; they also get printed out to the console as follows:

```sh
...
Sector:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Sector:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
TOTALS       Seek:  0  Rotate:105  Transfer: 60  Total: 165
```

Now let's do an example with a seek. Try the following set of requests:

```sh
$ python3 ./disk.py -a 10,18 -G
```

To compute how long this will take, you need to know how long a seek will take. The distance between each track is by default 40 distance units, and the default rate of seeking is 1 distance unit per unit time. Thus, a seek from the outer track to the middle track takes 40 time units. 

You'd also have to know the scheduling policy. The default is FIFO, though, so for now you can just compute the request times assuming the processing order matches the list specified via the `-a` flag.

To compute how long it will take the disk to service these requests, we first compute how long it takes to access sector 10, which we know from above to be 135 time units (105 rotating, 30 transferring). Once this request is complete, the disk begins to seek to the middle track where sector 18 lies, taking 40 time units. Then the disk rotates to sector 18, and transfers it for 30 time units, thus completing the simulation. But how long does this final rotation take?

To compute the rotational delay for 18, first figure out how long the disk would take to rotate from the end of the access to sector 10 to the beginning of the access to sector 18, assuming a zero-cost seek. As you can see from the simulator, sector 10 on the outer track is lined up with sector 22 on the middle track, and there are 7 sectors separating 22 from 18 (23, 12, 13, 14, 15, 16, and 17, as the disk spins counter-clockwise). Rotating through 7 sectors takes 210 time units (30 per sector). However, the first part of this rotation is actually spent seeking to the middle track, for 40 time units. Thus, the actual rotational delay for accessing sector 18 is 210 minus 40, or 170 time units. Run the simulator to see this for yourself; note that you can run without graphics and with the "-c" flag to just see the results without seeing the graphics.

```sh
$ python3 ./disk.py -a 10,18 -c
...
Sector:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Sector:  18  Seek: 40  Rotate:170  Transfer: 30  Total: 240
TOTALS       Seek: 40  Rotate:275  Transfer: 60  Total: 375
```

Some examples, involving flags, that you might find helpful while solving this lab are:

1) `-a 21,22,17`: Adds request for sectors 21, 22 and 17 in the queue.
2) `-S 4`: Sets seek rate to 4.
3) `-R 3`: Sets rotation rate to 3.
4) `-p FIFO`: Sets Disk scheduling policy to first-in-first-out (FIFO).
5) `-w 5`: Changes scheduling window size to 5 (for some scheduling policies).

You are encouraged to go through disk.py code and explore the implementation furthur. You should now have a basic idea of how the simulator works. The questions provided along with this lab, will explore some of the different options, to better help you build a model of how a disk really works. The link to the original README can be found [here](https://github.com/remzi-arpacidusseau/ostep-homework/blob/master/file-disks/README.md).

## Deliverables

You should use the provided disk simulator and answer the questions on [Moodle](https://moodle.iitd.ac.in/mod/quiz/view.php?id=112812). Note that you will have only one attempt which can be submitted before the deadline. You can start the attempt to check the questions and continue the attempt to answer them at a later point of time before the deadline. **Answers can not be modified and no re-attempts are allowed once this part is submitted**. So, submit this part only when you are completely sure.

# Logistics

- **Submission Deadline:** The final date for submitting your lab is February 3, 2024, at 11:59 PM.
- **Part-A Grading:** Part-A of the lab carries a total of 25 marks, contributing 5% to the overall course grade.
- **Part-B Grading:** Part-B of the lab is worth 4 marks and accounts for 1% of the total course grade.

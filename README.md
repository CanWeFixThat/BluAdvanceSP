# BluAdvanceSP

BluAdvanceSP is a project, that allows you to mod your **GameBoy Advance SP** with *Bluetooth*. 
To make this happen, we use a commonly used Bluetooth transmitter module called **KCX_ BT_ EMITTER**. 

We could use it solo and call it a day. This would be kinda boring and I wanted to have something more sophisticated. When we connect it directly to the *GBA SP* it will drain unnecessary its battery; even when we won't use it.

To get around this problem we could modify the case and add an extra switch to turn the module off; not ideal, but a solution. The next issue would be the pairing process itself, a second button would be needed, or you could connect it for example to the *select* button of the GameBoy. But this would mean, every time you press this button it will start the pairing process of the module - not very smart!

Thats why I was thinking about another solution: **A microcontroller!**

I used an **ATTiny85** from *Atmel* to control the Bluetooth module in a smart way.
This tiny little masterpiece allows us to extend the functionality of the brightness control button of the *GBA SP*.

We don't sacrifice any functionality of the button itself, it can still be used to turn *on/off* the background light of the screen. But furthermore we can power *on/off* the Bluetooth module and activate its *pairing* process if needed.
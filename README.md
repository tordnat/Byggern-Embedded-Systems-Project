# Embedded_Systems_TTK4155
TTK4155 - Embedded and Industrial Computer Systems Design  

Byggern'!


## Structure

├── docs  
├── LICENSE.meta  
├── node1  
├── node2  
├── node3  
│     ├── can_forwarder  
│     ├── test_data  
│     ├── tracking  
└── README.md  

### Summary
The 3 nodes communicate over a CAN bus. Node 1 is an Atmega162, Node 2 is an ATSAM3X8, Node 3 is an Arduino Uno connected over serial to a computer running OpenCV. Node 3 is an extra node which is not part of the curriculum.

## Visual Feedback with OpenCV

The third node uses OpenCV to detect the position of the ball which is forwarded to node 2. 


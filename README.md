# Major2
Names: 
Dan Shadel
Logan Falkensten
Eli Cruse
Jared Westmoreland

Organization: The project was organized into pairs, with two people handling the server, and two people handling the client. Dan and Jared handled the client side, and Logan and Eli the server side. 
Specifically, Logan handled adding integer values and client maximum, and Eli handled status updates and the listening protocols for additional clients.
Dan handled the transfer over to the client being a 'server' as well as the disconnect and sending of integers to the server. Jared handled the quit command and status updates and messages. 

Design Overview: The server side is fairly straight forward, with main listening and determining how many clients are connected, threads them into the function, and establishes the connection.
The server_function function listens for the clients commands and performs the integer increment function until it hits the required threshold, then passes functionality to the respective client. 


Complete specification: The biggest amiguity in the specification we had to deal with was being unsure of whether the client was supposed to maintain itself as a 'server' indefinitely until terminated.
Which, after futher reading into the requirements file, we determined that it was only meant to sustain itself briefly by returning the final number total from the other client. 

Known Bugs or Problems: Currently, the only bug that is known to us is some small formatting issues.

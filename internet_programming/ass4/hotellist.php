<!DOCTYPE HTML>
<html>
<head>
<title>hotel list</title>
</head>

<body>
<table>
<tr>
    <th>Price</th>
    <th>150</th>
    <th>200</th>
    <th>200</th>
</tr>
<tr>
    <td>Rooms Available</td>
<?php

include "hotel.php";

# connect to hotelgw
$socket = connect();

if($socket == NULL)
    echo "Cant connect to the hotel server";
else {
    $out = '';
    $bla = '';
    
    # send list command to hotelgw
    socket_write($socket, "list", 4);

    # retrieve list output
    while($out = socket_read($socket, 2048))
        $bla .= $out;

    # split input on tab
    $rooms = explode("\t", $bla);

    # print the values as link in the table
    foreach ($rooms as $value) {
        echo "    <td>$value</td>\n";
    }

}
?>
</tr>
</table>
</body>
</html>

<!DOCTYPE HTML>
<html>
<head>
<title>hotel participants</title>
</head>

<body>
<?php

include "hotel.php";

# connect to hotelgw
$socket = connect();

if($socket == NULL)
    echo "Cant connect to the hotel server";
else {
    $quests_req = "guests";
    $response = '';
    $out = '';

    socket_write($socket, $quests_req, strlen($quests_req));

    while($out = socket_read($socket, 2048))
        $response .= $out;

    $guests = explode("'", $response);

    for($i = 0; $i < count($guests); $i++)
        if(($i  % 2) != 0)
            echo $guests[$i] . "<br />";
}

?>



</body>
</html>

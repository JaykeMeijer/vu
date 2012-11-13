<!DOCTYPE HTML>
<html>
<head>
<title>hotel book</title>
</head>

<body>
<?php 
if(isset($_POST['type']) && isset($_POST['name'])):

    include "hotel.php";

    # connect to hotelgw
    $socket = connect();

    if($socket == NULL)
        echo "Cant connect to the hotel server";
    else {
        $book_req = "book " .$_POST['type'] . " '" . $_POST['name'] . "'";
        $response = '';
        $out = '';

        socket_write($socket, $book_req, strlen($book_req));

        while($out = socket_read($socket, 2048))
            $response .= $out;

        if($response == "true")
            echo "Booking confirmed";
        else
            echo "Booking denied";
    }
else:
?>

<form name="input" action="hotelbook.php" method="post">
Type:
<select name="type">
<option value="1">1: $150</option>
<option value="2">2: $200</option>
<option value="3">3: $200</option>
</select><br />
Name: <input type="text" name="name"><br />
<input type="submit" value="Submit">
</form>

<?php endif; ?>
    
</body
</html>

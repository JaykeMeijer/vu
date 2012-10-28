<?php

require_once 'setup.php';

function connect()
{
    global $HOTEL_GATEWAY_ADDR, $HOTEL_GATEWAY_PORT;

    if(($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) == false)
        return NULL;
    else if(($result = socket_connect($socket, $HOTEL_GATEWAY_ADDR, $HOTEL_GATEWAY_PORT)) == false)
        return NULL;
    else
        return $socket;
    

}

?>

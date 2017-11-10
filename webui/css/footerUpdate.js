if(typeof(EventSource)!=="undefined"){
    var source = new EventSource("status");
    source.addEventListener('status',
        function (event) {
            if (document.getElementById("FOOTER_RIGHT").innerHTML != event.data &&
                document.getElementById("FOOTER_RIGHT").innerHTML != ""){
                
                showNotification("Server is now " + event.data);
            }

            if( event.data == "busy"){
                document.getElementById("FOOTER_LEFT").innerHTML = '<span class="glyphicon glyphicon-remove" onclick="stop();" title="Stop"></span>'
            }else{
                document.getElementById("FOOTER_LEFT").innerHTML = ""
            }

            document.getElementById("FOOTER_RIGHT").innerHTML = event.data;
        }, false);

    source.onmessage = function(event) {
        document.getElementById("FOOTER_RIGHT").innerHTML = "Unknown event:" + event.data;
    };
        
}else{
    document.getElementById("FOOTER_RIGHT").innerHTML = "N/A"; 
}



function stop(){
    document.getElementById("BS_ALERT").innerHTML = '<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>Canceled';
    document.getElementById("BS_ALERT").className = "alert alert-danger alert-dismissable";
    
    document.getElementById("FOOTER_LEFT").innerHTML += '<iframe width="1" height="1" frameborder="0" src="stopBiogeme?pw=password"></iframe>';
}

var Notification = window.Notification || window.mozNotification || window.webkitNotification;
if(Notification){
    if(Notification.permission !== "denied"){
        if(Notification.permission !== "granted"){
            document.getElementById("FOOTER_CENTER").innerHTML = '<span title="Allow notification" class="glyphicon glyphicon-comment" onclick="requestPerm();"></span>'
        }else{
            document.getElementById("FOOTER_CENTER").className += " invisible";
            document.getElementById("FOOTER_CENTER").innerHTML = "Permission already granted";
        }
    }else{
        document.getElementById("FOOTER_CENTER").className += " invisible";
        document.getElementById("FOOTER_CENTER").innerHTML = "Notifications denied";
    }
}else{
    document.getElementById("FOOTER_CENTER").className += " invisible";
    document.getElementById("FOOTER_CENTER").innerHTML = "Notifications not supported";
}


function requestPerm(){
    // Request permission
    //alert("Hello");
    Notification.requestPermission(function (status) {

        if (Notification.permission !== status) {
            Notification.permission = status;
        }
    });
}


function theNotification(text) {
    
    var n = new Notification("Biogeme webui",  {
      tag: 'note', 
      body: text //+ " " + window.navigator.userAgent
    });

    setTimeout(function () {
        n.close()
    }, 4000);
}





function showNotification(text) {
    // If notifications are granted show the notification
    if (Notification && Notification.permission === "granted") {
        theNotification(text);
    }

    // If they are not denied (i.e. default)
    else if (Notification && Notification.permission !== "denied") {
        //alert("request");

        // Request permission
        Notification.requestPermission(function (status) {

            // Change based on user's decision
            if (Notification.permission !== status) {
                Notification.permission = status;
            }

            // If it's granted show the notification
            if (status === "granted") {
                theNotification(text);
            }

            else {
            // Back up if the user's browser doesn't support notifications
            }

        });

    }

    else {
    // Back up if the user's browser doesn't support notifications
    }
}
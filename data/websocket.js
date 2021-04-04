var connection = new WebSocket('ws://' + location.hostname + '/ws');
 
connection.onopen = function () {
	connection.send(window.location.pathname + " connected");
};

connection.onclose = function () {
	connection.send(window.location.pathname + " disconnected");
};

connection.onerror = function (error) {
	window.alert('WebSocket Error ', error);
};

connection.onmessage = function (e) {
	var path = window.location.pathname;
	var page = path.split('/').pop();
	var s = '' + e.data;
	console.log(s);

	// name=wert: Ersetzung von Teilen der HTML-Seite (dynamische Updates)
	var element = s.split("=");

	if (element.length == 2 && !s.includes("|")) {
		var htmlElement = document.getElementById(element[0]);
		if (htmlElement != null) document.getElementById(element[0]).innerHTML = element[1];
		
	} else {
		element = s.split('|');
		var oldTableContent = document.getElementById("tableContent").innerHTML;
		var newEntry = "<tr>";
		newEntry += "<td>" + element[0] + "</td>";
		newEntry += "<td>" + new Date().toLocaleTimeString() + "</td>";
		newEntry += "<td>" +  element[1] + "ms</td>";
		newEntry += "<td>" +  element[2] + "</td>";
		newEntry += "<td>" +  element[3] + "</td>";
		newEntry += "</tr>\n";
		document.getElementById("tableContent").innerHTML = newEntry + oldTableContent;
	}
	
};
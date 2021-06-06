// ----------------------------------------------------------------------------------------------------
//

function pomwrite(addr, cv, val) {
	console.log("POM Write: CV" + cv + "=" + val);
	connection.send("pomWrite|" + addr + "|" + cv + "|" + val);
}

// ----------------------------------------------------------------------------------------------------
//

function cvwrite(cv, val) {
	console.log("CV Write: CV" + cv + "=" + val);
	connection.send("cvWrite|" + cv + "|" + val);
}


function cvread(cv) {
	console.log("CV Read: CV" + cv);
}

function cvreadresult(success, val) {
	console.log("CV Read result: CV" + cv + " " + val);
}

// ----------------------------------------------------------------------------------------------------
// Wenn Bit-Checkbox gesetzt, wird in zugehöriger CV das Bit ebenfalls gesetzt, ansonsten gelöscht
// Die CV hat die Id "CV#", z.B. CV29
// Die Bitcheckbox die Id "BitCV.#", z.B. Bit29.1

function checkboxClicked(cv, bit) {
	cvid = "CV" + cv;
	bitid = "Bit" + cv + "." + bit;
	cvval = document.getElementById(cvid).value;
	mask = 1 << bit;
	if (document.getElementById(bitid).checked) {
		cvval |= mask; // Bit setzen
	} else {
		cvval &= ~mask; // Bit löschen
	} 
	document.getElementById(cvid).value = cvval;
}

function radioboxClicked(cv, bit) {
	console.log("Clicked: Radio " + bit + " for CV" + cv);
}


//----------------------------------------------------------------------------------------------------
// Wenn CV-Wert direkt geändert wird, prüfen, ob Bit-CV und dann Bits mitändern

function cvChanged(cv, val) {

	cvid = "CV" + cv;
	bitid = "Bit" + cv + ".0";
	cvval = document.getElementById(cvid).value;

	element = document.getElementById(bitid);
    if(typeof(element) != 'undefined' && element != null) {
		for (i=0; i<8; i++) {
			mask = 1 << i;
			bitid = "Bit" + cv + "." + i;
			document.getElementById(bitid).checked = ((val & mask) > 0);
		}
	}
}
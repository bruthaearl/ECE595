const db_url = 'http://18.224.222.33:3000/';
const db_setpoint = db_url + 'setpoints';
const db_setpoint_sorted = db_url +'setpoints?_sort=time_h,time_m&_order=asc';
const db_stats = db_url + 'stats'
const db_stats_patch = db_url + 'stats/1'
const db_setpoint_delete = db_url + 'setpoints/'
const setpoints_table = document.getElementById('setpoints_table'); 

//prevent enter key from submitting form 
window.addEventListener('keydown',function(e){
	if(e.keyIdentifier=='U+000A'||e.keyIdentifier=='Enter'||e.keyCode==13){
		if(e.target.nodeName=='INPUT'&&e.target.type=='number'){
			e.preventDefault();
			return false;
		}
	}
},true);


//Update DOM helper
function updateElement(element, newstuff){
	document.getElementById(element).innerHTML = newstuff;
	return 0;
}

//Functions to create scheduled setpoints table
function createNode(element){
	return document.createElement(element);	
}
function append(parent, element){
	return parent.appendChild(element);
}


//Update time
var current_time = new Date();
current_hours = current_time.getHours();
current_minutes = current_time.getMinutes();
if(current_minutes.toString().length == 1){
	current_minutes = "0" + current_minutes;
}
updateElement("time", current_hours+":"+current_minutes);


//Load setpoints
fetch(db_setpoint_sorted)
  	.then(response => response.json())
  	.then(data => {
  		let setpoints = data;
		return setpoints.map((setpoint)=>{
			// Create node for each setpoint
			let row = createNode('tr');
			let id_td = createNode('td');
			let setpoint_td = createNode('td');
			let time_td = createNode('td');
			let time_h = setpoint.time_h;
			let time_m = setpoint.time_m;
			// Make things pretty
			if(time_h.toString().length == 1){
				time_h = "0"+time_h;
			}
			if(time_m.toString().length == 1){
				time_m = "0"+time_m;
			}
			// Load data into nodes
			id_td.innerHTML = setpoint.id;
			setpoint_td.innerHTML = setpoint.temp + "&degC";
			time_td.innerHTML = time_h + ":" + time_m;
			// Add nodes to DOM
			append(row, id_td);
			append(row, setpoint_td);
			append(row, time_td);
			append(setpoints_table, row);
		})	
  	})
  	.catch((error)=> {
    	console.log(error);
  	});   

//Load stats
fetch(db_stats)
  	.then(response => response.json()) 
  	.then(data => {
  		let stats = data;
		return stats.map((stat)=>{
			updateElement("temp", stat.current_temp + "&degC");
			updateElement("setpoint", stat.current_setpoint + "&degC");

			if(stat.heater_status){ 
				updateElement("hstat", "ON");
				document.getElementById("hstat").style.color = "#D35400";
			}
			else{
				updateElement("hstat", "OFF");
				document.getElementById("hstat").style.color = "#3498DB";
			}		
		})	
  	})
  	.catch((error)=> {
    	console.log(error);
  	}); 


//Verify setpoint form
function verifySetpoint(temp, time_h, time_m){
	if(isNaN(temp) || temp < 0 || temp > 100){
		return 0;
	}
	if(isNaN(time_h) || time_h < 0 || time_h > 24){
		return 0;
	}
	if(isNaN(time_m) || time_m < 0 || time_m > 60){
		return 0;
	}
	return 1;
}


//Submit scheduled setpoint form
function submitScheduledSetpoint(){

	let temp_temp = parseInt(document.getElementById("new_temp").value);
	let temp_time_h = parseInt(document.getElementById("new_hour").value);
	let temp_time_m = parseInt(document.getElementById("new_minute").value);

	var verified = verifySetpoint(temp_temp, temp_time_h, temp_time_m);

	if(!verified){
		alert("Invalid Numbers");
		return -1;
	}

	var post_data = new Object();
	post_data["temp"] = temp_temp;
	post_data["time_h"] = temp_time_h;
	post_data["time_m"] = temp_time_m;
	console.log(JSON.stringify(post_data));

	fetch(db_setpoint, {
  		method: 'post',
		headers: {
    		'Accept': 'application/json, text/plain, */*',
		    'Content-Type': 'application/json'
  		},
  		body: JSON.stringify(post_data)
	})
	.then(res=>res.json())
  	.then(res => console.log(res));

    setTimeout(reFresh, 500);
}


//submit manual setpoint form
function submitSetpoint(){

	let setpoint_temp = parseInt(document.getElementById("new_manual_setpoint").value);
	console.log(setpoint_temp);
	var verified = verifySetpoint(setpoint_temp, 1, 1);

	if(!verified){
		alert("Invalid Numbers");
		return -1;
	}

	var post_data = new Object();
	post_data["current_setpoint"] = setpoint_temp;

	console.log(JSON.stringify(post_data));

	fetch(db_stats_patch, {
  		method: 'PATCH',
		headers: {
    		'Accept': 'application/json, text/plain, */*',
		    'Content-Type': 'application/json'
  		},
  		body: JSON.stringify(post_data)
	})
	.then(res=>res.json())
  	.then(res => console.log(res));

   	setTimeout(reFresh, 500);
}


// delete setpoint form
function deleteSetpoint(){

	let delete_id = parseInt(document.getElementById("delete_field").value);
	console.log(delete_id);

	if(isNaN(delete_id)){
		alert("Invalid ID");
		return -1;
	}

	var delete_data = new Object();
	delete_url = db_setpoint_delete + delete_id;
	console.log(delete_url);

	fetch(delete_url, {
  		method: 'DELETE',
		headers: {
    		'Accept': 'application/json, text/plain, */*',
		    'Content-Type': 'application/json'
  		},
  //		body: JSON.stringify(post_data)
	})
	.then(res=>res.json())
  	.then(res => console.log(res));

  	setTimeout(reFresh, 500);


  //	location.reload(true);
}

// hide fields until button is clicked
function showInputField(id){

	switch(id){
		case 1:
		document.getElementById("manual_set_form").style.display = "inline";
		break;
		case 2:
		document.getElementById("delete_form").style.display = "inline";
		break;
		default:
		case 3:
		document.getElementById("sch_set_form").style.display = "inline";
		break;
	}
}

// timeout to ensure data gets sent
function reFresh(){
	location.reload(true);
}
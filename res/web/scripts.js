
const db_setpoint = 'http://localhost:3000/setpoints';
const db_setpoint_sorted = 'http://localhost:3000/setpoints?_sort=time_h,time_m&_order=asc';
const db_stats = 'http://localhost:3000/stats'
const setpoints_table = document.getElementById('setpoints_table'); 

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
updateElement("time", current_time.getHours()+":"+current_time.getMinutes());

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


//Submit setpoint form
function submitSetpoint(){

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

  	location.reload(true);
}
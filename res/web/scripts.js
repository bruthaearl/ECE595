
const db_setpoint = 'http://localhost:3000/setpoints';
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
fetch(db_setpoint)
  	.then(response => response.json())
  	.then(data => {
  		let setpoints = data;
		return setpoints.map((setpoint)=>{
			// Create node for each setpoint
			let row = createNode('tr');
			let id_td = createNode('td');
			let setpoint_td = createNode('td');
			let time_td = createNode('td');
			// Load data into nodes
			id_td.innerHTML = setpoint.id;
			setpoint_td.innerHTML = setpoint.temp;
			time_td.innerHTML = setpoint.time_h + ":" + setpoint.time_m;
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
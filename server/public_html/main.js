/* 
 * Read CSV file, draw charts on page
 * Listen for nav clicks, scroll to click
 * Required: d3.v3.js
*/


// set up canvas size
var margin = {top: 20, right: 20, bottom: 30, left: 50};
var width = 960 - margin.left - margin.right;
var height = 500 - margin.top - margin.bottom;

 
// load in csv file, parse it, and draw a chart for each var we want
d3.csv("data.csv", function(error, data) {

	data.forEach(function(d) {
		d.date = new Date( d.date * 1000 ); // js epoch time is in ms, so Arduino code must be * 1000
	});
 
 	// populate latest stats
 	d3.select("#temp_now").text( data[ data.length - 1 ].temp + "\u00B0 F" );
 	d3.select("#light_now").text( data[ data.length - 1 ].light + "%" );
 	d3.select("#soil_now").text( data[ data.length - 1 ].soil + "%" );
 
	createBarChart(data, "temp");
	createBarChart(data, "soil");
	createBarChart(data, "light");    

});
 
 
function createBarChart( data, csvColName, divID ) {

	// set up scales
	var minDate = d3.min(data, function(d) { return d.date; });
	var maxDate = d3.max(data, function(d) { return d.date; });
	var minY = d3.min(data, function(d) { return d[ csvColName ]; });
	var maxY = d3.max(data, function(d) { return parseFloat(d[csvColName]); });

	var x = d3.time.scale()
		.domain([ minDate, maxDate ])
		.range([ 0, width ]);	
	var y = d3.scale.linear()
		.domain([ minY-2, maxY+2 ]) // give some padding to min, max
		.range([height, 0]);

	// set up axes
	var xAxis = d3.svg.axis()
	    .scale(x)
	    .orient("bottom")
		.ticks(8);
 
	var yAxis = d3.svg.axis()
	    .scale(y)
	    .orient("left")
		.ticks(10);


	// draw the chart base
	var svg = d3.select("#"+csvColName).append("svg")
	    .attr("width", width + margin.left + margin.right)
	    .attr("height", height + margin.top + margin.bottom)
	  .append("g")
	    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

	drawRules(svg, x, y );
 
    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);
 
    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis)
        .append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 6)
        .attr("dy", ".71em")
        .style("text-anchor", "end");

	// draw the scaled paths / data points
	var line = d3.svg.line()
	    .x(function(d) { return x( d.date ); })
	    .y(function(d) { return y( d[csvColName] ); });
 
    svg.append("path")
        .datum(data)
        .attr("class", "line")
        .attr("d", line);

}


function drawRules( svg, x, y ) {
	
	// bg lines container
	var vis = svg.append("svg:g");
	var rules = vis.append("svg:g").classed("rules", true)

	function make_x_axis() {
	  return d3.svg.axis()
	      .scale(x)
	      .orient("bottom")
	      .ticks(8)
	}

	function make_y_axis() {
	  return d3.svg.axis()
	      .scale(y)
	      .orient("left")
	      .ticks(5)
	}

	rules.append("svg:g").classed("grid x_grid", true)
	    .attr("transform", "translate(0,"+height+")")
	    .call(make_x_axis()
	      .tickSize(-height,0,0)
	      .tickFormat("")
	    )

	rules.append("svg:g").classed("grid y_grid", true)
	    .call(make_y_axis()
	      .tickSize(-width,0,0)
	      .tickFormat("")
	    )
	
}
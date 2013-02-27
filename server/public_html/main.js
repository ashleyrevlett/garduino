
var margin = {top: 20, right: 20, bottom: 30, left: 50},
    width = 960 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;
 

d3.csv("data.csv", function(error, data) {

//	console.log(data);
//	console.log(error);	

	data.forEach(function(d) {
		d.date = new Date( d.date * 1000 );
	});
 
	createBarChart(data, "temp");
	createBarChart(data, "soil");
	createBarChart(data, "light");    

});
 
 
function createBarChart( data, csvColName, divID ) {

	var minDate = d3.min(data, function(d) { return d.date; });
	var maxDate = d3.max(data, function(d) { return d.date; });
	var minY = d3.min(data, function(d) { return d[ csvColName ]; });
	var maxY = d3.max(data, function(d) { return d[ csvColName ]; });

	var x = d3.time.scale()
		.domain([ minDate, maxDate ])
		.range([ 0, width ]);	
	var y = d3.scale.linear()
		.domain([ minY, maxY ])
		.range([height, 0]);

	var xAxis = d3.svg.axis()
	    .scale(x)
	    .orient("bottom");
 
	var yAxis = d3.svg.axis()
	    .scale(y)
	    .orient("left");

	var line = d3.svg.line()
	    .x(function(d) { return x( d.date ); })
	    .y(function(d) { return y( d[csvColName] ); });
 
	var svg = d3.select("#"+csvColName).append("svg")
	    .attr("width", width + margin.left + margin.right)
	    .attr("height", height + margin.top + margin.bottom)
	  .append("g")
	    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
 
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
        .style("text-anchor", "end")
        .text( csvColName );
 
    svg.append("path")
        .datum(data)
        .attr("class", "line")
        .attr("d", line);

}
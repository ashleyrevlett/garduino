		
d3.text("data.csv", function(unparsedData) {

	// read the csv file, cut it down to most recent 100 rows
	var dataset = d3.csv.parseRows(unparsedData);
	var totalReadings = 100;			
	dataset = dataset.splice( dataset.length - totalReadings, totalReadings );

	// must multiply by 1000 to convert c++ epoch to js epoch
	dataset.forEach(function(d) {
		d[0] = d[0]*1000; 
	});

	createBarChart(dataset, 1, "#temp");
	createBarChart(dataset, 3, "#light");
	createBarChart(dataset, 4, "#soil_moisture");
	createBarChart(dataset, 2, "#humidity");
				
}); // end .text

		
function createBarChart( dataset, csvColumnNumber, divID ) {
	
	// prepare the chart graphics
	var barPadding = 0;	
	var w = 760;
	var h = 400;
	var pad = 50;

	// set up ranges and scales
	var minDate = d3.min(dataset, function(d) { return new Date(d[0]); });
	var maxDate = d3.max(dataset, function(d) { return new Date(d[0]); });
	var minTemp = d3.min(dataset, function(d) { return d[ csvColumnNumber ]; });
	var maxTemp = d3.max(dataset, function(d) { return d[ csvColumnNumber ]; });
	var xScale = d3.time.scale().domain([minDate, maxDate]).range([0,w]);
	var yScale = d3.scale.linear().domain([minTemp, maxTemp]).range([h,0]);

	// draw the graph
	var svg = d3.select( divID )
	    .append("svg:svg")
	    .attr("height", h + pad)
	    .attr("width",  w + pad)

	// draw lines container
	var vis = svg.append("svg:g")
	    .attr("transform", "translate(40,20)")
	var rules = vis.append("svg:g").classed("rules", true)

	// draw x and y axis
	function make_x_axis() {
	  return d3.svg.axis()
	      .scale(xScale)
	      .orient("bottom")
	      .ticks(8)
	}
	function make_y_axis() {
	  return d3.svg.axis()
	      .scale(yScale)
	      .orient("left")
	      .ticks(10)
	}
	
	// draw grid
	rules.append("svg:g").classed("grid x_grid", true)
	    .attr("transform", "translate(0,"+h+")")
	    .call(make_x_axis()
	      .tickSize(-h,0,0)
	      .tickFormat("")
	    )
	rules.append("svg:g").classed("grid y_grid", true)
	    .call(make_y_axis()
	      .tickSize(-w,0,0)
	      .tickFormat("")
	    )
	rules.append("svg:g").classed("labels x_labels", true)
	    .attr("transform", "translate(0,"+h+")")
	    .call(make_x_axis()
	      .tickSize(10)
	      .tickFormat(d3.time.format("%I:%M %p"))
	    )
	rules.append("svg:g").classed("labels y_labels", true)
	    .call(make_y_axis()
	      .tickSubdivide(1)
	      .tickSize(10, 5, 0)
	    )
		
	// draw bars
	svg.append("svg:g").classed("bar_chart container", true)
		.attr("transform", "translate(40,0)")
		.selectAll("rect")
		.data(dataset)
		.enter()
		.append("rect")
		.attr("fill", "#cccccc")
		.style("opacity", 0.9)
		.attr("x", function(d,i) {
			return xScale(d[0]);
		})
		.attr("y", function(d,i) {
			return ( h - yScale(d[ csvColumnNumber ]) + 20 );
		})
		.attr("width", function(d,i) {
			var barWidth = (w - (pad*2)) / dataset.length;
			return (barWidth);
		})
		.attr("height", function(d,i) {
			return yScale(d[ csvColumnNumber ]);						
		});
			
}
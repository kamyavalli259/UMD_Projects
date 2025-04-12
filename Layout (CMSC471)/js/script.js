// Larger canvas this time
const width = 1200;
const height = 800;

function createVis(data) {
   const color = d3.scaleOrdinal(d3.schemeCategory10);

  // The force simulation mutates links and nodes, so create a copy
  // so that re-evaluating this cell produces the same result.
  const links = data.links.map(d => ({...d}));
  const nodes = data.nodes.map(d => ({...d}));

  // Create a simulation with several forces.
  const simulation = d3.forceSimulation(nodes)
      .force("link", d3.forceLink(links).id(d => d.id))
      .force("charge", d3.forceManyBody())
      .force("center", d3.forceCenter(width / 2, height / 2))
      .on("tick", ticked);

  // Create the SVG container.
  const svg = d3.select("#vis").append("svg")
      .attr("width", width)
      .attr("height", height)
      .attr("viewBox", [0, 0, width, height])
      .attr("style", "max-width: 100%; height: auto;");

  // Add a line for each link, and a circle for each node.
  const link = svg.append("g")
      .attr("stroke", "#999")
      .attr("stroke-opacity", 0.6)
    .selectAll()
    .data(links)
    .join("line")
      .attr("stroke-width", d => Math.sqrt(d.value));

  const node = svg.append("g")
      .attr("stroke", "#fff")
      .attr("stroke-width", 1.5)
    .selectAll()
    .data(nodes)
    .join("circle")
      .attr("r", 5)
      .attr("fill", d => color(d.group));

  node.append("title")
      .text(d => d.id);

  // Add a drag behavior.
  node.call(d3.drag()
        .on("start", dragstarted)
        .on("drag", dragged)
        .on("end", dragended));
    
  //
  // Define drag behavior functions


  // Set the position attributes of links and nodes each time the simulation ticks.
  function ticked() {
    link
        .attr("x1", d => d.source.x)
        .attr("y1", d => d.source.y)
        .attr("x2", d => d.target.x)
        .attr("y2", d => d.target.y);

    node
        .attr("cx", d => d.x)
        .attr("cy", d => d.y);
  }

  // Reheat the simulation when drag starts, and fix the subject position.
  function dragstarted(event) {
    if (!event.active) simulation.alphaTarget(0.3).restart();
    event.subject.fx = event.subject.x;
    event.subject.fy = event.subject.y;
  }

  // Update the subject (dragged node) position during drag.
  function dragged(event) {
    event.subject.fx = event.x;
    event.subject.fy = event.y;
  }

  // Restore the target alpha so the simulation cools after dragging ends.
  // Unfix the subject position now that it’s no longer being dragged.
  function dragended(event) {
    if (!event.active) simulation.alphaTarget(0);
    event.subject.fx = null;
    event.subject.fy = null;
  }
  
}


function createTree(data) {
    const width = 928;
    const marginTop = 10;
    const marginRight = 10;
    const marginBottom = 10;
    const marginLeft = 40;
  
    const root = d3.hierarchy(data);
    const dx = 10;
    const dy = (width - marginRight - marginLeft) / (1 + root.height);
  
    const tree = d3.tree().nodeSize([dx, dy]);
    const diagonal = d3.linkHorizontal().x(d => d.y).y(d => d.x);
  
    const svg = d3.select("#vis2").append("svg")
        .attr("width", width)
        .attr("height", dx)
        .attr("viewBox", [-marginLeft, -marginTop, width, dx])
        .attr("style", "max-width: 100%; height: auto; font: 10px sans-serif; user-select: none;");
  
    const gLink = svg.append("g")
        .attr("fill", "none")
        .attr("stroke", "#555")
        .attr("stroke-opacity", 0.4)
        .attr("stroke-width", 1.5);
  
    const gNode = svg.append("g")
        .attr("cursor", "pointer")
        .attr("pointer-events", "all");
  
    const drag = d3.drag()
        .on("start", function(event, d) {
          // store the initial position of the dragged node
          d3.select(this).raise().classed("active", true);
        })
        .on("drag", function(event, d) {
          // Move the node based on drag event
          d.x += event.dy;
          d.y += event.dx;
          d3.select(this)
            .attr("transform", `translate(${d.y},${d.x})`);
          update(event, d); // Call update to adjust the layout
        })
        .on("end", function(event, d) {
          d3.select(this).classed("active", false);
        });
  
    function update(event, source) {
      const duration = event?.altKey ? 2500 : 250;
      const nodes = root.descendants().reverse();
      const links = root.links();
  
      tree(root);
  
      let left = root;
      let right = root;
      root.eachBefore(node => {
        if (node.x < left.x) left = node;
        if (node.x > right.x) right = node;
      });
  
      const height = right.x - left.x + marginTop + marginBottom;
  
      const transition = svg.transition()
          .duration(duration)
          .attr("height", height)
          .attr("viewBox", [-marginLeft, left.x - marginTop, width, height])
          .tween("resize", window.ResizeObserver ? null : () => () => svg.dispatch("toggle"));
  
      const node = gNode.selectAll("g")
        .data(nodes, d => d.id);
  
      const nodeEnter = node.enter().append("g")
          .attr("transform", d => `translate(${source.y0},${source.x0})`)
          .attr("fill-opacity", 0)
          .attr("stroke-opacity", 0)
          .on("click", (event, d) => {
            d.children = d.children ? null : d._children;
            update(event, d);
          })
          .call(drag); // Attach drag behavior to the nodes
  
      nodeEnter.append("circle")
          .attr("r", 2.5)
          .attr("fill", d => d._children ? "#555" : "#999")
          .attr("stroke-width", 10);
  
      nodeEnter.append("text")
          .attr("dy", "0.31em")
          .attr("x", d => d._children ? -6 : 6)
          .attr("text-anchor", d => d._children ? "end" : "start")
          .text(d => d.data.name)
          .attr("stroke-linejoin", "round")
          .attr("stroke-width", 3)
          .attr("stroke", "white")
          .attr("paint-order", "stroke");
  
      const nodeUpdate = node.merge(nodeEnter).transition(transition)
          .attr("transform", d => `translate(${d.y},${d.x})`)
          .attr("fill-opacity", 1)
          .attr("stroke-opacity", 1);
  
      const nodeExit = node.exit().transition(transition).remove()
          .attr("transform", d => `translate(${source.y},${source.x})`)
          .attr("fill-opacity", 0)
          .attr("stroke-opacity", 0);
  
      const link = gLink.selectAll("path")
        .data(links, d => d.target.id);
  
      const linkEnter = link.enter().append("path")
          .attr("d", d => {
            const o = {x: source.x0, y: source.y0};
            return diagonal({source: o, target: o});
          });
  
      link.merge(linkEnter).transition(transition)
          .attr("d", diagonal);
  
      link.exit().transition(transition).remove()
          .attr("d", d => {
            const o = {x: source.x, y: source.y};
            return diagonal({source: o, target: o});
          });
  
      root.eachBefore(d => {
        d.x0 = d.x;
        d.y0 = d.y;
      });
    }
  
    root.x0 = dy / 2;
    root.y0 = 0;
    root.descendants().forEach((d, i) => {
      d.id = i;
      d._children = d.children;
      if (d.depth && d.data.name.length !== 7) d.children = null;
    });
    update(null, root);
  }
  


function init() {
  d3.json("./data/miserables.json")
    .then((data) => {
      console.log(data);
      createVis(data);
      
    })
    .catch((error) => console.error("Error loading data:", error));



    d3.json("./data/flare-2.json")
    .then((data) => {
      console.log(data);
      createTree(data);
    })
    .catch((error) => console.error("Error loading data:", error));


    d3.json("data/simple.json").then(function(data) {
        createTree(data);
      }).catch(function(error) {
        console.error("Error loading the data: ", error);
      });
      

}


// Load the JSON data from the 'data/simple.json' file


window.addEventListener("load", init);

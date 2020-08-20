// Graphics Objects
var gl;
var shaderProgram;

// HTML IDs
const CANVAS_ID = "#glCanvas";
const HOUSE_SLIDER_ID = "#numHousesOut";
const STORY_SLIDER_ID = "#maxStoryOut";

// Scene Parameters
var numHouses;
var maxStories;
var aspectRatio;

// Default Parameters
const DEF_NUM_HOUSES = 3;
const DEF_MAX_STORIES = 5;

// Color Palette

const SKY_COLOR = vec3(0.4, 0.9, 1.0);
const GROUND_COLOR = vec3(0.6, 0.4, 0.3);
const SUN_COLOR = vec3(0.8, 0.8, 0.1);
const CLOUD_COLOR = vec3(0.8, 0.9, 0.9);
const TREE_LEAVES_COLOR = vec3(0.2, 0.6, 0.2);
const TREE_BARK_COLOR = vec3(0.5, 0.1, 0.1);
const WINDOW_COLOR = vec3(0.4, 0.6, 1.0);
const ROOF_COLOR = vec3(0.8, 0.1, 0.1);

const BUILDING_COLORS = [
    vec3(1.0, 0.0, 0.0), // Red
    vec3(0.0, 1.0, 0.0), // Green
    vec3(0.0, 0.0, 1.0), // Blue
    vec3(1.0, 1.0, 0.0), // Yellow
    vec3(1.0, 0.0, 1.0), // Purple
    vec3(0.4, 0.4, 0.4)  // Gray
];

// Buffer Data
var vertices = [];
var colors = [];
var indices = [];

// Buffers
var vertexBuffer;
var colorBuffer;
var indexBuffer;

// Quad vertices mapped to two triangles

const TRIANGLE_INDICES = [0, 1, 2];
const QUAD_INDICES = [0, 1, 2, 0, 2, 3];

// Scene Objects

// Background objects

// Sky
var skyV;
var skyC;
var skyI;

// Ground
var groundV;
var groundC;
var groundI;

// Sun
const SUN_RESOLUTION = 32;
var sunV;
var sunC;
var sunI;

// Clouds
var canDraw = false;
var currCloud = [];
var cloudV = [];
var cloudC = [];
var cloudI = [];

// Calculated Objects: Buildings and Trees
const GROUND_LEVEL = -0.8;
const WIDTH_MODIFIER = 4 / 5;
const TREE_WIDTH_MODIFIER = 1 / 8;
const LEAVES_RESOLUTION = 32;

// Spacing and Height Parameters
var storyHeight;
var xSpacing;

// Building Generation Parameters
var bStart;
var bWidth;
var bStories = [];
var bColors = [];

// Tree Generation Parameters
var tStart;
var tHeight;
var tWidth;

// Initializer function
window.onload = function init() {
    // Get the canvas from the HTML file
    var canvas = document.querySelector(CANVAS_ID);
    aspectRatio = canvas.width / canvas.height;

    // Initialize the GL context
    gl = WebGLUtils.setupWebGL(canvas);

    // Check if WebGL is available and working
    if (!gl) {
        alert("Something went wrong fetching WebGL, stopping execution.");
    }

    //  Load shaders and initialize attribute buffers
    shaderProgram = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(shaderProgram);

    // Initialize scene parameters
    updateHouses(DEF_NUM_HOUSES);
    updateStories(DEF_MAX_STORIES);

    // Attach click listeners

    // Left click listener to add cloud vertices
    canvas.addEventListener("click", function(e) {
        if (!canDraw) {
            return;
        }

        // Add current coordinates to polygon
        currCloud[currCloud.length] = vec2(
            2 * e.clientX / canvas.width - 1,
            -2 * e.clientY / canvas.height + 1
        );
    });

    // Right click listener to finish cloud being drawn
    canvas.addEventListener("contextmenu", function(e) {
        e.preventDefault();

        if (!canDraw || currCloud.length < 3) {
            return;
        }

        // Append last cloud to cloud arrays
        var indexOffset = cloudV.length;
        var currIndex;

        for (i = 0; i < currCloud.length; i++) {
            currIndex = cloudV.length;
            cloudV[currIndex] = currCloud[i];
            cloudC[currIndex] = CLOUD_COLOR;
        }

        // Append a triangle fan index scheme
        for (i = 1; i < currCloud.length - 1; i++) {
            cloudI[cloudI.length] = indexOffset;
            cloudI[cloudI.length] = indexOffset + i;
            cloudI[cloudI.length] = indexOffset + i + 1;
        }

        // Reset cloud holder
        currCloud = [];

        // Update scene
        render();
    });

    // Configure WebGL
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(1.0, 1.0, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    // Create and Associate Buffers with Shaders
    vertexBuffer = gl.createBuffer();
    colorBuffer = gl.createBuffer();
    indexBuffer = gl.createBuffer();
    associateBuffers();
};

// Graphics Updaters

// Scene renderer
function render() {
    // Clear canvas
    gl.clear(gl.COLOR_BUFFER_BIT);

    // Update buffers
    compileBufferData();
    updateBuffers();

    // Draw
    gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_SHORT, 0);

    console.log("Temp");
}

// Creates and adds buffer data for background objects
function initBackground() {
    // Sky
    skyV = quadFactory(vec2(-1, -1), vec2(1, 1));
    skyC = uniformColorFactory(SKY_COLOR, 4);
    skyI = QUAD_INDICES;

    // Ground
    groundV = quadFactory(vec2(-1, -1), vec2(1, -0.4));
    groundC = uniformColorFactory(GROUND_COLOR, 4);
    groundI = QUAD_INDICES;

    // Sun
    sunV = circleVertexFactory(vec2(0.75, 0.7), 0.1, SUN_RESOLUTION);
    sunC = uniformColorFactory(SUN_COLOR, SUN_RESOLUTION);
    sunI = circleIndexFactory(SUN_RESOLUTION);
}

// Associates buffers with shaders
function associateBuffers() {
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
    var vPosition = gl.getAttribLocation(shaderProgram, "vPosition");
    gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);

    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    var color = gl.getAttribLocation(shaderProgram, "color");
    gl.vertexAttribPointer(color, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(color);
}

// Updates buffer data
function updateBuffers() {
    // Vertex
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bufferData(
        gl.ARRAY_BUFFER, flatten(vertices), gl.STATIC_DRAW);

    // Color
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.bufferData(
        gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);

    // Index
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
    gl.bufferData(
        gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);
}

// Compiles scene data from various sources to put into buffers
function compileBufferData() {
    // Add background objects
    initBackground();
    appendArrays(skyV, skyC, skyI);
    appendArrays(groundV, groundC, groundI);
    appendArrays(sunV, sunC, sunI);

    // Add clouds
    appendArrays(cloudV, cloudC, cloudI);

    // Add buildings and trees
    generateBuildings();
    generateTrees();
}

// Appends given vertex, color and index arrays to buffer data
function appendArrays(arrayV, arrayC, arrayI) {
    // Helper variables
    var indexOffset = vertices.length;
    var currIndex;

    // Append vertex and color vectors
    for (k = 0; k < arrayV.length; k++) {
        currIndex = vertices.length;
        vertices[currIndex] = arrayV[k];
        colors[currIndex] = arrayC[k];
    }

    // Append indices
    for (k = 0; k < arrayI.length; k++) {
        indices[indices.length] = arrayI[k] + indexOffset;
    }
}

// Scene Parameter and Slider Value Updaters

// Updates data related to number of houses when corresponding slider is clicked
function updateHouses(val) {
    document.querySelector(HOUSE_SLIDER_ID).value = val;
    numHouses = val;
    updateStories(maxStories);

    // Update relevant values
    xSpacing = 2.0 / (2 * numHouses); // Leave space at sides
    bStart = 1;
    tStart = 2;
    bWidth = xSpacing * WIDTH_MODIFIER;
    tWidth = bWidth * TREE_WIDTH_MODIFIER;
}

// Updates data related to number of stories when corresponding slider is clicked
function updateStories(val) {
    document.querySelector(STORY_SLIDER_ID).value = val;
    maxStories = val;

    // Update relevant values
    storyHeight = 0.2;
}

// Buttons

// Generates random scene with previously selected parameters
function generate() {
    resetScene();
    canDraw = true;
    for (p = 0; p < numHouses; p++) {
        bStories.push(Math.floor(Math.random() * (maxStories - 1)) + 1);
        bColors[p] = Math.floor(Math.random() * BUILDING_COLORS.length);
    }

    render();
}

/*
    Mostly taken from: https://stackoverflow.com/questions/13405129/javascript-create-and-save-file#30832210
    Creates a downloadable .txt file that holds necessary the information
    necessary to load the scene from the save
*/
function save() {
    // Save bStories[] and bColors[]
    var data = [
        maxStories + "\n" + numHouses + "\n" +
        bStories + "\n" + bColors + "\n" +
        cloudV + "\n" + cloudC + "\n" + cloudI];

    var file = new Blob(data, {type: "text/plain"});

    var a = document.createElement("a"), url = URL.createObjectURL(file);
    a.href = url;
    a.download= "save.txt";
    document.body.appendChild(a);
    a.click();
    setTimeout(function() {
        document.body.removeChild(a);
        window.URL.revokeObjectURL(url);
    }, 0);
}

/*
    Gathered from StackOverflow and Mozilla Developer Network
    Loads scene from the save file selected via the file explorer button
*/
function load() {

    // Reset scene
    resetScene();
    canDraw = true;

    var file = document.querySelector("#fileExplorer").files[0];

    if (!file) {
        alert("Failed to load file.");
    }
    else {
        var reader = new FileReader();
        reader.onload = function(event) {
            var contents = event.target.result;
            var lines = contents.split("\n");

            // Restore parameters
            updateStories(parseInt(lines[0]));
            updateHouses(parseInt(lines[1]));

            // Restore bStories
            var stories = lines[2].split(",");
            console.log("stories: " + stories);
            for (i = 0; i < stories.length; i++) {
                bStories[i] = parseInt(stories[i]);
                console.log(bStories[i])
            }

            // Restore bColors
            var colors = lines[3].split(",");
            console.log("colors: " + colors);
            for (i = 0; i < colors.length; i++) {
                bColors[i] = parseInt(colors[i]);
                console.log(bColors[i]);
            }

            // Restore clouds if save contains clouds
            if (lines.length > 4) {
                var temp;
                var reset = false;

                var cV = lines[4].split(",");
                cloudV = [];

                for (i = 0; i < cV.length; i++) {
                    temp = parseFloat(cV[i]);

                    if (isNaN(temp)) {
                        reset = true;
                        break;
                    }

                    cloudV[i] = temp;
                }

                var cC = lines[5].split(",");
                cloudC = [];

                if (!reset) {
                    for (i = 0; i < cC.length; i++) {
                        temp = parseFloat(cC[i]);

                        if (isNaN(temp)) {
                            reset = true;
                            break;
                        }

                        cloudC[i] = temp;
                    }
                }

                var cI = lines[6].split(",");
                cloudI = [];

                // Deal with empty lines creating NaN cloud data by resetting
                if (!reset) {
                    for (i = 0; i < cI.length; i++) {
                        temp = parseInt(cI[i]);

                        if (isNaN(temp)) {
                            reset = true;
                            break;
                        }

                        cloudI[i] = temp;
                    }
                }

                // Pack cloud vertices into vec2
                cV = [];

                for (i = 0; i < cloudV.length; i+= 2) {
                    cV[i / 2] = vec2(cloudV[i], cloudV[i+1]);
                }

                cloudV = cV;

                // Pack cloud colors into vec3
                cC = [];

                for (i = 0; i < cloudC.length; i+=3) {
                    cC[i / 3] = vec3(cloudC[i], cloudC[i+1], cloudC[i+2]);
                }

                cloudC = cC;

                if (reset) {
                    cloudV = [];
                    cloudC = [];
                    cloudI = [];
                }
            }
            else {
                cloudV = [];
                cloudC = [];
                cloudI = [];
            }

            render();
        };

        reader.onerror = function () {
            alert("Failed to read the file.");
        };

        reader.readAsText(file);
    }
}

// Resets the scene information
function resetScene() {
    vertices = [];
    colors = [];
    indices = [];

    currCloud = [];
    cloudV = [];
    cloudC = [];
    cloudI = [];

    bStories = [];
    bColors = [];
}

// Scene Object Builders

// Generates all the building in the scene and pushes the data into buffers
function generateBuildings() {
    var midX;
    var upY;
    var lowerLeft;
    var upperRight;

    var wallsV = [];
    var wallsC = [];
    var wallsI = [];

    for (i = 0; i < numHouses; i++) {
        midX = (bStart + 2 * i) * xSpacing - 1;
        lowerLeft = vec2(
            midX - bWidth / 2,
            GROUND_LEVEL
        );

        upY = GROUND_LEVEL + (bStories[i] * storyHeight);
        upperRight = vec2(midX + bWidth/2, upY);

        // Append
        wallsV = quadFactory(lowerLeft, upperRight);
        wallsC = uniformColorFactory(BUILDING_COLORS[bColors[i]], 4);
        wallsI = QUAD_INDICES;
        appendArrays(wallsV, wallsC, wallsI);

        // Add roof
        wallsV = [
            vec2(lowerLeft[0], upperRight[1]),
            vec2(midX, upperRight[1] + storyHeight),
            upperRight];
        wallsC = uniformColorFactory(ROOF_COLOR, 3);
        wallsI = TRIANGLE_INDICES;
        appendArrays(wallsV, wallsC, wallsI);

        // Add windows
        for (z = 0; z < bStories[i]; z++) {
            generateWindows(lowerLeft[0], midX, upperRight[0], z);
        }
    }
}

// Helper function to generate windows for the buildings
function generateWindows(leftX, midX, rightX, story) {
    var lY = (storyHeight * 2 / 8) + story * storyHeight + GROUND_LEVEL;
    var uY = (storyHeight * 6 / 8) + story * storyHeight + GROUND_LEVEL;
    var llX = (1 * leftX + 6 * midX) / 7;
    var lrX = (6 * leftX + 1 * midX) / 7;
    var rlX = (6 * midX + 1 * rightX) / 7;
    var rrX = (1 * midX + 6 * rightX) / 7;

    // Append left window
    appendArrays(
        quadFactory(vec2(llX, lY), vec2(lrX, uY)),
        uniformColorFactory(WINDOW_COLOR, 4),
        QUAD_INDICES
    );

    // Append right windows
    appendArrays(
        quadFactory(vec2(rlX, lY), vec2(rrX, uY)),
        uniformColorFactory(WINDOW_COLOR, 4),
        QUAD_INDICES
    );
}

// Generates all the trees in the scene and pushes the data into buffers
function generateTrees() {
    var midX;
    var upY;
    var lowerLeft;
    var upperRight;
    var center;
    for (a = 0; a < numHouses - 1; a++) {
        midX = (tStart + 2 * a) * xSpacing - 1;
        lowerLeft = vec2(
            midX - tWidth / 2,
            GROUND_LEVEL
        );

        upY = GROUND_LEVEL + storyHeight + tWidth;
        upperRight = vec2(midX + tWidth/2, upY);

        // Append trunk
        appendArrays(
            quadFactory(lowerLeft, upperRight),
            uniformColorFactory(TREE_BARK_COLOR, 4),
            QUAD_INDICES
        );

        // Append leaves
        center = vec2(midX, upY);
        appendArrays(
            circleVertexFactory(center, xSpacing / 3, LEAVES_RESOLUTION),
            uniformColorFactory(TREE_LEAVES_COLOR, LEAVES_RESOLUTION),
            circleIndexFactory(LEAVES_RESOLUTION)
        );
    }
}

// Creates a vertex array for a rectangle
function quadFactory(lowLeft, upRight) {
    var vertices = [
        lowLeft, vec2(upRight[0], lowLeft[1]),
        upRight, vec2(lowLeft[0], upRight[1])];

    return vertices;
}

// Creates a color array made up of a single given color repeated
function uniformColorFactory(vec, count) {
    var colors = [];

    for (j = 0; j < count; j++)
    {
        colors[j] = vec;
    }

    return colors;
}

// Creates a circle approximation polygon and returns a vertex array
function circleVertexFactory(center, radius, resolution) {
    var vertices = [center];
    var angle = 0;
    var step = 2 * Math.PI / (resolution - 1);

    var currX;
    var currY;

    // Generate points of the approximating polygon
    for (i = 0; i < resolution - 1; i++) {
        currX = center[0] + (radius * Math.cos(angle));

        // Multiply by aspect ratio to turn ellipses into circles
        // This is caused by transforming cubic model to rectangle canvas
        currY = center[1] + (radius * Math.sin(angle) * aspectRatio);

        // Append
        vertices[vertices.length] = vec2(currX, currY);

        // Iterate
        angle += step;
    }

    return vertices;
}

/*
    Creates an index array to be used with a circle approximating vertex array
    that creates a logically TRIANGLE_FAN pattern
*/
function circleIndexFactory(resolution) {
    var indices = [];

    // Basically keep the center fixed and shift between approximating triangles
    for (i = 1; i < resolution - 1; i++) {
        indices[indices.length] = 0;
        indices[indices.length] = i;
        indices[indices.length] = i + 1;
    }

    // Join the last and the first vertex to close the circle
    indices[indices.length] = 0;
    indices[indices.length] = resolution - 1;
    indices[indices.length] = 1;

    return indices;
}

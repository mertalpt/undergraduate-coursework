/*
    Author: Mert Alp Taytak
    ID: 21602061

    JavaScript file for Assignment 3 of the course CS465.
*/

/*
    CONSTANTS and GLOBAL VARIABLES
*/

// WebGL object
var GL;

// Structure to access GL programs
const GL_PROGRAMS = {
    WIREFRAME: null,
    GOURAUD: null,
    PHONG: null
};

// Selections
var SELECTION_SHAPE;
var SELECTION_SHADER;
var SELECTION_MAPPING;

// Superquadric object parameters
var ETA_RESOLUTION;
var OMEGA_RESOLUTION;
var EPSILON_1;
var EPSILON_2;
var ALPHA_1;
var ALPHA_2;
var ALPHA_3;
var ALPHA_4;

// Buffers
var BUFFER_VERTICES;
var BUFFER_NORMALS;

// Buffer data
const DATA_VERTICES = [];
const DATA_NORMALS = [];

// Textures
var TEXTURE_0;

var DATA_TEXTURE = [];

var TEXTURE_SIZE = 16;

var TEXTURE_IMAGE_HELPER = new Array()
for (var i =0; i<TEXTURE_SIZE; i++)  
    TEXTURE_IMAGE_HELPER[i] = new Array();
for (var i =0; i<TEXTURE_SIZE; i++) 
    for ( var j = 0; j < TEXTURE_SIZE; j++) 
        TEXTURE_IMAGE_HELPER[i][j] = new Float32Array(4);
for (var i =0; i<TEXTURE_SIZE; i++) 
    for (var j=0; j<TEXTURE_SIZE; j++) {
        var c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0));
        TEXTURE_IMAGE_HELPER[i][j] = [Math.abs(Math.cos(c)), Math.abs(Math.sin(c)), c, 1];
}

var TEXTURE_IMAGE = new Uint8Array(4*TEXTURE_SIZE*TEXTURE_SIZE);

    for ( var i = 0; i < TEXTURE_SIZE; i++ ) 
        for ( var j = 0; j < TEXTURE_SIZE; j++ ) 
           for(var k =0; k<4; k++) 
                TEXTURE_IMAGE[4*TEXTURE_SIZE*i+4*j+k] = 255*TEXTURE_IMAGE_HELPER[i][j][k];

// Data locations
var LOC_VERTICES;
var LOC_NORMALS;
var LOC_TEXTURES;

// Uniform data
var UNI_MODELVIEW;
var UNI_PROJECTION;
var UNI_NORMAL_MATRIX;

// Uniform locations
var LOC_MODELVIEW;
var LOC_PROJECTION;
var LOC_AMBIENT;
var LOC_DIFFUSE;
var LOC_SPECULAR;
var LOC_LIGHT_POS;
var LOC_SHININESS;
var LOC_NORMAL_MATRIX;

// ModelView parameters
var MV_EYE = vec3();
const MV_AT = vec3(0.0, 0.0, 0.0);
var MV_UP = vec3(0.0, 1.0, 0.0);
var MV_RADIUS = 20;
var MV_THETA = 0;
var MV_PHI = 0;
const MV_RADIUS_MOD = 0.1;
const MV_ANGLE_MOD = Math.PI / 30;

// Projection parameters
var PROJ_FOV = 45.0;
var PROJ_ASPECT = 1.0;
var PROJ_NEAR = -10;
var PROJ_FAR = 0;

// Lights
const LIGHT_POS = vec4(5.0, -10.0, -20.0, 0.0);
const LIGHT_AMBIENT = vec4(0.2, 0.2, 0.2, 1.0);
const LIGHT_DIFFUSE = vec4(1.0, 1.0, 1.0, 1.0);
const LIGHT_SPECULAR = vec4(1.0, 1.0, 1.0, 1.0);

// Material
const MATERIAL_AMBIENT = vec4(1.0, 1.0, 1.0, 1.0);
const MATERIAL_DIFFUSE = vec4(0.6, 0.6, 0.6, 1.0);
const MATERIAL_SPECULAR = vec4(0.5, 0.5, 0.5, 1.0);
const MATERIAL_SHININESS = 5.0;

// Products
const PROD_AMBIENT = mult(LIGHT_AMBIENT, MATERIAL_AMBIENT);
const PROD_DIFFUSE = mult(LIGHT_DIFFUSE, MATERIAL_DIFFUSE);
const PROD_SPECULAR = mult(LIGHT_SPECULAR, MATERIAL_SPECULAR);

// Mouse listener variables
var MOUSE_X_START;
var MOUSE_X_END;
var MOUSE_Y_START;
var MOUSE_Y_END;
var MOUSE_DRAGGED;

// Render flag
var HAS_INITIALIZED = false;

// Set up program start
window.onload = initialize;

/**
 * Initializes WebGL, compiles shaders and sets up global variables.
 */
function initialize()
{
    // Call listeners first to initialize labels and global variables
    listener_shape();
    listener_shading();
    listener_mapping();
    listener_gamma();
    listener_phi();
    listener_theta();
    listener_eta();
    listener_omega();
    listener_epsilon1();
    listener_epsilon2();
    listener_alpha1();
    listener_alpha2();
    listener_alpha3();
    listener_radius();

    // Get WebGL context
    const canvas = document.getElementById("gl_canvas");
    GL = canvas.getContext("webgl");

    if (GL === null) {
        alert("Unable to initialize WebGL.");
    }

    // Set canvas mouse listeners
    // canvas.onmousedown = function(event) {
    //     MOUSE_DRAGGED = true;
    //     let rect = canvas.getBoundingClientRect();
    //     let mousePos = {x: event.clientX - rect.left, y: event.clientY - rect.top};
    //     MOUSE_X_START = mousePos.x;
    //     MOUSE_Y_START = mousePos.y;
    // };

    // canvas.onmousemove = function(event) {
    //     // Only act when mouse is being pressed       
    //     if (!MOUSE_DRAGGED) {
    //         return;
    //     }
    //     let rect = canvas.getBoundingClientRect();
    //     let mousePos = {x: event.clientX - rect.left, y: event.clientY - rect.top};
    //     MOUSE_X_END = mousePos.x;
    //     MOUSE_Y_END = mousePos.y;

    //     if (MOUSE_X_END - MOUSE_X_START > 10 || MOUSE_Y_END - MOUSE_Y_START > 10) {
    //         // Apply rotation
    //         trackball_apply_rotation();
    //         render();

    //         // Switch end and start
    //         MOUSE_X_START = MOUSE_X_END;
    //         MOUSE_Y_START = MOUSE_Y_END;
    //     }        
    // };

    // canvas.onmouseup = function(event) {
    //     MOUSE_DRAGGED = false;
    // };

    canvas.onwheel = function(event) {
        // Temporarily disable page scroll
        event.preventDefault();

        // Calculate radius change
        let delta = event.deltaY;

        if (delta > 0) // Zoom out
            MV_RADIUS = MV_RADIUS * (1 + 1*MV_RADIUS_MOD);
        else // Zoom in
            MV_RADIUS = MV_RADIUS * (1 - 1*MV_RADIUS_MOD);

        // Display changes
        render();
    };

    // Set some WebGL parameters
    GL.viewport(0, 0, canvas.width, canvas.height);
    GL.clearColor(0.8, 0.8, 0.8, 1.0);
    GL.enable(GL.DEPTH_TEST);
    GL.clear(GL.COLOR_BUFFER_BIT);

    // Compile shaders and programs
    var wireframe = initShaders(GL, "wireframe_v_shader", "wireframe_f_shader");
    var gouraud = initShaders(GL, "gouraud_v_shader", "gouraud_f_shader");
    var phong = initShaders(GL, "phong_v_shader", "phong_f_shader");

    // Write to the enum
    GL_PROGRAMS.WIREFRAME = wireframe;
    GL_PROGRAMS.GOURAUD = gouraud;
    GL_PROGRAMS.PHONG = phong;

    if (
        GL_PROGRAMS.WIREFRAME === null || 
        GL_PROGRAMS.GOURAUD === null || 
        GL_PROGRAMS.PHONG === null) {
            alert("One of the shaders could not be initialized.");
    }

    // Create buffers
    BUFFER_VERTICES = GL.createBuffer();
    BUFFER_NORMALS = GL.createBuffer();

    // Create textures
    TEXTURE_0 = GL.createTexture();  

    // Create first render
    HAS_INITIALIZED = true;
    render();
}

/**
 * Renders the scene.
 */
function render()
{
    if (!HAS_INITIALIZED) {
        return;
    }

    GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT);

    // Delegate to other functions based on user selection
    if (SELECTION_SHADER == 0) // Wireframe
        render_wireframe();
    else if (SELECTION_SHADER == 1) // Gouraud
        render_gouraud();
    else if (SELECTION_SHADER == 2) // Phong
        render_phong();
    else
        alert("Something went wrong with shader selection.");
}

/**
 * Renders the scene as a wireframe.
 */
function render_wireframe()
{
    // Select appropriate program
    GL.useProgram(GL_PROGRAMS.WIREFRAME);

    // Get locations of attributes and uniforms
    LOC_VERTICES = GL.getAttribLocation(GL_PROGRAMS.WIREFRAME, "vPos");
    LOC_MODELVIEW = GL.getUniformLocation(GL_PROGRAMS.WIREFRAME, "modelViewMatrix");
    LOC_PROJECTION = GL.getUniformLocation(GL_PROGRAMS.WIREFRAME, "projectionMatrix");

    // Generate vertices
    if (SELECTION_SHAPE == 0) // Ellipsoid
        ellipsoid_vertices();
    else if (SELECTION_SHAPE == 1) // Toroid
        toroid_vertices();
    else
        alert("Something went wrong with shape selection.");

    // Bind buffer data
    GL.bindBuffer(GL.ARRAY_BUFFER, BUFFER_VERTICES);
    GL.bufferData(GL.ARRAY_BUFFER, flatten(DATA_VERTICES), GL.STATIC_DRAW);
    GL.vertexAttribPointer(LOC_VERTICES, 4, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_VERTICES);  

    // Calculate uniforms
    MV_EYE = vec3(
        MV_RADIUS * Math.sin(MV_THETA) * Math.cos(MV_PHI),
        MV_RADIUS * Math.sin(MV_THETA) * Math.sin(MV_PHI),
        MV_RADIUS * Math.cos(MV_THETA)
    );
    UNI_MODELVIEW = lookAt(MV_EYE, MV_AT, MV_UP);
    UNI_PROJECTION = perspective(PROJ_FOV, PROJ_ASPECT, PROJ_NEAR, PROJ_FAR);

    // Bind uniforms
    GL.uniformMatrix4fv(LOC_MODELVIEW, false, flatten(UNI_MODELVIEW));
    GL.uniformMatrix4fv(LOC_PROJECTION, false, flatten(UNI_PROJECTION));

    // Draw
    for (let i = 0; i < DATA_VERTICES.length; i += 3) {
        GL.drawArrays(GL.LINE_LOOP, i, 3);
    }
}

function render_gouraud()
{
    // Select appropriate program
    GL.useProgram(GL_PROGRAMS.GOURAUD);

    // Get locations of attributes and uniforms
    LOC_VERTICES = GL.getAttribLocation(GL_PROGRAMS.GOURAUD, "vPos");
    LOC_NORMALS = GL.getAttribLocation(GL_PROGRAMS.GOURAUD, "vNormal");
    LOC_TEXTURES = GL.getAttribLocation(GL_PROGRAMS.GOURAUD, "vTexCoord");
    LOC_MODELVIEW = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "modelViewMatrix");
    LOC_PROJECTION = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "projectionMatrix");
    LOC_AMBIENT = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "ambientProduct");
    LOC_DIFFUSE = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "diffuseProduct");
    LOC_SPECULAR = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "specularProduct");
    LOC_LIGHT_POS = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "lightPos");
    LOC_SHININESS = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "shininess");
    LOC_NORMAL_MATRIX = GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "normalMatrix");

    // Generate vertices, normals and textures
    if (SELECTION_SHAPE == 0) { // Ellipsoid
        ellipsoid_vertices();
        ellipsoid_normals();
        ellipsoid_textures();
    }
    else if (SELECTION_SHAPE == 1) { // Toroid
        toroid_vertices();
        toroid_normals();
        toroid_textures();
    }
    else
        alert("Something went wrong with shape selection.");

    // Bind buffer data 
    GL.bindBuffer(GL.ARRAY_BUFFER, BUFFER_VERTICES);
    GL.bufferData(GL.ARRAY_BUFFER, flatten(DATA_VERTICES), GL.STATIC_DRAW);
    GL.vertexAttribPointer(LOC_VERTICES, 4, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_VERTICES);

    GL.bindBuffer(GL.ARRAY_BUFFER, BUFFER_NORMALS);
    GL.bufferData(GL.ARRAY_BUFFER, flatten(DATA_NORMALS), GL.STATIC_DRAW);
    GL.vertexAttribPointer(LOC_NORMALS, 4, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_NORMALS);

    GL.vertexAttribPointer(LOC_TEXTURES, 2, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_TEXTURES);
    configureTexture();
    GL.activeTexture(GL.TEXTURE0);
    GL.bindTexture(GL.TEXTURE_2D, TEXTURE_0);
    GL.uniform1i(GL.getUniformLocation(GL_PROGRAMS.GOURAUD, "tex0"), 0);

    // Calculate uniforms
    MV_EYE = vec3(
        MV_RADIUS * Math.sin(MV_THETA) * Math.cos(MV_PHI),
        MV_RADIUS * Math.sin(MV_THETA) * Math.sin(MV_PHI),
        MV_RADIUS * Math.cos(MV_THETA)
    );
    UNI_MODELVIEW = lookAt(MV_EYE, MV_AT, MV_UP);
    UNI_PROJECTION = perspective(PROJ_FOV, PROJ_ASPECT, PROJ_NEAR, PROJ_FAR);
    UNI_NORMAL_MATRIX = [
        vec3(UNI_MODELVIEW[0][0], UNI_MODELVIEW[0][1], UNI_MODELVIEW[0][2]),
        vec3(UNI_MODELVIEW[1][0], UNI_MODELVIEW[1][1], UNI_MODELVIEW[1][2]),
        vec3(UNI_MODELVIEW[2][0], UNI_MODELVIEW[2][1], UNI_MODELVIEW[2][2])
    ];

    // Bind uniforms
    GL.uniformMatrix4fv(LOC_MODELVIEW, false, flatten(UNI_MODELVIEW));
    GL.uniformMatrix4fv(LOC_PROJECTION, false, flatten(UNI_PROJECTION));
    GL.uniformMatrix3fv(LOC_NORMAL_MATRIX, false, flatten(UNI_NORMAL_MATRIX));
    GL.uniform4fv(LOC_AMBIENT, flatten(PROD_AMBIENT));
    GL.uniform4fv(LOC_DIFFUSE, flatten(PROD_DIFFUSE));
    GL.uniform4fv(LOC_SPECULAR, flatten(PROD_SPECULAR));
    GL.uniform4fv(LOC_LIGHT_POS, flatten(LIGHT_POS));
    GL.uniform1f(LOC_SHININESS, MATERIAL_SHININESS);

    // Draw
    for (let i = 0; i < DATA_VERTICES.length; i += 3) {
        GL.drawArrays(GL.TRIANGLES, i, 3);
    }
}

function render_phong()
{
    // Select appropriate program
    GL.useProgram(GL_PROGRAMS.PHONG);

    // Get locations of attributes and uniforms
    LOC_VERTICES = GL.getAttribLocation(GL_PROGRAMS.PHONG, "vPos");
    LOC_NORMALS = GL.getAttribLocation(GL_PROGRAMS.PHONG, "vNormal");
    LOC_TEXTURES = GL.getAttribLocation(GL_PROGRAMS.PHONG, "vTexCoord");
    LOC_MODELVIEW = GL.getUniformLocation(GL_PROGRAMS.PHONG, "modelViewMatrix");
    LOC_PROJECTION = GL.getUniformLocation(GL_PROGRAMS.PHONG, "projectionMatrix");
    LOC_AMBIENT = GL.getUniformLocation(GL_PROGRAMS.PHONG, "ambientProduct");
    LOC_DIFFUSE = GL.getUniformLocation(GL_PROGRAMS.PHONG, "diffuseProduct");
    LOC_SPECULAR = GL.getUniformLocation(GL_PROGRAMS.PHONG, "specularProduct");
    LOC_LIGHT_POS = GL.getUniformLocation(GL_PROGRAMS.PHONG, "lightPos");
    LOC_SHININESS = GL.getUniformLocation(GL_PROGRAMS.PHONG, "shininess");
    LOC_NORMAL_MATRIX = GL.getUniformLocation(GL_PROGRAMS.PHONG, "normalMatrix");

    // Generate vertices and normals
    if (SELECTION_SHAPE == 0) { // Ellipsoid
        ellipsoid_vertices();
        ellipsoid_normals();
    }
    else if (SELECTION_SHAPE == 1) { // Toroid
        toroid_vertices();
        toroid_normals();
    }
    else
        alert("Something went wrong with shape selection.");

    // Bind buffer data
    GL.bindBuffer(GL.ARRAY_BUFFER, BUFFER_VERTICES);
    GL.bufferData(GL.ARRAY_BUFFER, flatten(DATA_VERTICES), GL.STATIC_DRAW);
    GL.vertexAttribPointer(LOC_VERTICES, 4, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_VERTICES);

    GL.bindBuffer(GL.ARRAY_BUFFER, BUFFER_NORMALS);
    GL.bufferData(GL.ARRAY_BUFFER, flatten(DATA_NORMALS), GL.STATIC_DRAW);
    GL.vertexAttribPointer(LOC_NORMALS, 4, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_NORMALS);

    GL.vertexAttribPointer(LOC_TEXTURES, 2, GL.FLOAT, false, 0, 0);
    GL.enableVertexAttribArray(LOC_TEXTURES);
    configureTexture();
    GL.activeTexture(GL.TEXTURE0);
    GL.bindTexture(GL.TEXTURE_2D, TEXTURE_0);
    GL.uniform1i(GL.getUniformLocation(GL_PROGRAMS.PHONG, "tex0"), 0);

    // Calculate uniforms
    MV_EYE = vec3(
        MV_RADIUS * Math.sin(MV_THETA) * Math.cos(MV_PHI),
        MV_RADIUS * Math.sin(MV_THETA) * Math.sin(MV_PHI),
        MV_RADIUS * Math.cos(MV_THETA)
    );
    UNI_MODELVIEW = lookAt(MV_EYE, MV_AT, MV_UP);
    UNI_PROJECTION = perspective(PROJ_FOV, PROJ_ASPECT, PROJ_NEAR, PROJ_FAR);
    UNI_NORMAL_MATRIX = [
        vec3(UNI_MODELVIEW[0][0], UNI_MODELVIEW[0][1], UNI_MODELVIEW[0][2]),
        vec3(UNI_MODELVIEW[1][0], UNI_MODELVIEW[1][1], UNI_MODELVIEW[1][2]),
        vec3(UNI_MODELVIEW[2][0], UNI_MODELVIEW[2][1], UNI_MODELVIEW[2][2])
    ];

    // Bind uniforms
    GL.uniformMatrix4fv(LOC_MODELVIEW, false, flatten(UNI_MODELVIEW));
    GL.uniformMatrix4fv(LOC_PROJECTION, false, flatten(UNI_PROJECTION));
    GL.uniformMatrix3fv(LOC_NORMAL_MATRIX, false, flatten(UNI_NORMAL_MATRIX));
    GL.uniform4fv(LOC_AMBIENT, flatten(PROD_AMBIENT));
    GL.uniform4fv(LOC_DIFFUSE, flatten(PROD_DIFFUSE));
    GL.uniform4fv(LOC_SPECULAR, flatten(PROD_SPECULAR));
    GL.uniform4fv(LOC_LIGHT_POS, flatten(LIGHT_POS));
    GL.uniform1f(LOC_SHININESS, MATERIAL_SHININESS);

    // Draw
    for (let i = 0; i < DATA_VERTICES.length; i += 3) {
        GL.drawArrays(GL.TRIANGLES, i, 3);
    }
}

/*
    TEXTURES
*/

function configureTexture()
{
    GL.activeTexture(GL.TEXTURE0);
    GL.bindTexture(GL.TEXTURE_2D, TEXTURE_0);
    GL.pixelStorei(GL.UNPACK_FLIP_Y_WEBGL, true);
    GL.texImage2D(
        GL.TEXTURE_2D, 0, GL.RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, 
        GL.RGBA, GL.UNSIGNED_BYTE, TEXTURE_IMAGE);
    GL.generateMipmap(GL.TEXTURE_2D);
    GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.NEAREST_MIPMAP_LINEAR);
    GL.texParameteri(GL.TEXTURE_2D, GL.TEXTURE_MAG_FILTER, GL.NEAREST);
}

/*
    TEXTURE COORDINATES, VERTEX and NORMAL GENERATORS
*/

/**
 * Takes the given array and function, applies the given function over a range
 * of superellipsoid function parameters to populate the array.
 * @param {array} array 
 * @param {function} func 
 */
function ellipsoid_generator(array, func)
{
    // Clear array
    array.length = 0;

    // Will be used to hold 4 points that will turn into 2 triangles
    let lowLeft, lowRight, upRight, upLeft;

    // Limits
    const ETA_HIGHER = Math.PI / 2;
    const ETA_LOWER = -Math.PI / 2;
    const OMEGA_HIGHER = Math.PI;
    const OMEGA_LOWER = -Math.PI;

    // Step sizes
    const ETA_STEP = (ETA_HIGHER - ETA_LOWER) / ETA_RESOLUTION;
    const OMEGA_STEP = (OMEGA_HIGHER - OMEGA_LOWER) / OMEGA_RESOLUTION;

    // Middle rows
    for (let i = 1; i <= ETA_RESOLUTION; i++) {
        let eta = i * ETA_STEP + ETA_LOWER;
        let etaPrev = (i - 1) * ETA_STEP + ETA_LOWER;

        for (let j = 0; j < OMEGA_RESOLUTION - 1; j++) {
            let omega = j * OMEGA_STEP + OMEGA_LOWER;
            let omegaNext = (j + 1) * OMEGA_STEP + OMEGA_LOWER;

            lowLeft = func(eta, omega);
            lowRight = func(eta, omegaNext);
            upRight = func(etaPrev, omegaNext);
            upLeft = func(etaPrev, omega);

            // Push triangles
            array.push(lowLeft);
            array.push(lowRight);
            array.push(upRight);
            array.push(lowLeft);
            array.push(upRight);
            array.push(upLeft);
        }

        // Connect last column to first column
        // Shift right to left for efficiency
        lowLeft = lowRight;
        upLeft = upRight;

        // Calculate first column
        lowRight = func(eta, OMEGA_LOWER);
        upRight = func(etaPrev, OMEGA_LOWER);

        // Push triangles
        array.push(lowLeft);
        array.push(lowRight);
        array.push(upRight);
        array.push(lowLeft);
        array.push(upRight);
        array.push(upLeft);
    }
}

/**
 * Parametric surface equation for superellipsoids.
 * @param {number} eta
 * @param {number} omega
 */
function ellipsoid_surface(eta, omega)
{
    let cosEta = Math.cos(eta);
    let sinEta = Math.sin(eta);
    let cosOmega = Math.cos(omega);
    let sinOmega = Math.sin(omega);
    
    // Cache for efficiency
    let cached = Math.sin(cosEta) * Math.pow(Math.abs(cosEta), EPSILON_1);

    return vec4(
        ALPHA_1 * cached * Math.sign(cosOmega) * Math.pow(Math.abs(cosOmega), EPSILON_2),
        ALPHA_2 * cached * Math.sign(sinOmega) * Math.pow(Math.abs(sinOmega), EPSILON_2),
        ALPHA_3 * Math.sign(sinEta) * Math.pow(Math.abs(sinEta), EPSILON_1),
        1.0
    );
}

/**
 * Parametric normal vector equation for superellipsoids.
 * @param {number} eta 
 * @param {number} omega 
 */
function ellipsoid_normal(eta, omega)
{
    let cosEta = Math.cos(eta);
    let sinEta = Math.sin(eta);
    let cosOmega = Math.cos(omega);
    let sinOmega = Math.sin(omega);
    
    // Cache for efficiency
    let cached = Math.sign(cosEta) * Math.pow(Math.abs(cosEta), 2 - EPSILON_1);

    return vec4(
        (1 / ALPHA_1) * cached * Math.sign(cosOmega) * Math.pow(Math.abs(cosOmega), 2 - EPSILON_2),
        (1 / ALPHA_2) * cached * Math.sign(sinOmega) * Math.pow(Math.abs(sinOmega), 2 - EPSILON_2),
        (1 / ALPHA_3) * Math.sign(sinEta) * Math.pow(Math.abs(sinEta), 2 - EPSILON_1),
        0.0
    );
}

function ellipsoid_texture(eta, omega)
{
    // Limits
    const ETA_HIGHER = Math.PI / 2;
    const ETA_LOWER = -Math.PI / 2;
    const OMEGA_HIGHER = Math.PI;
    const OMEGA_LOWER = -Math.PI;

    // Step sizes
    const ETA_STEP = (ETA_HIGHER - ETA_LOWER) / ETA_RESOLUTION;
    const OMEGA_STEP = (OMEGA_HIGHER - OMEGA_LOWER) / OMEGA_RESOLUTION;

    return vec2((eta - ETA_LOWER) / ETA_STEP, (omega - OMEGA_LOWER) - OMEGA_STEP);
}

function ellipsoid_vertices()
{
    ellipsoid_generator(DATA_VERTICES, ellipsoid_surface);
}

function ellipsoid_normals()
{
    ellipsoid_generator(DATA_NORMALS, ellipsoid_normal);
}

function ellipsoid_textures()
{
    ellipsoid_generator(DATA_TEXTURE, ellipsoid_texture);
}

/**
 * Takes the given array and function, applies the given function over a range
 * of supertoroid function parameters to populate the array.
 * @param {array} array 
 * @param {function} func 
 */
function toroid_generator(array, func)
{
    // Clear array
    array.length = 0;

    // Will be used to hold 4 points that will turn into 2 triangles
    let lowLeft, lowRight, upRight, upLeft;

    // Limits
    const ETA_LOWER = -Math.PI;
    const ETA_HIGHER = Math.PI;
    const OMEGA_LOWER = -Math.PI;
    const OMEGA_HIGHER = Math.PI;

    // Step size
    const ETA_STEP = (ETA_HIGHER - ETA_LOWER) / ETA_RESOLUTION;
    const OMEGA_STEP = (OMEGA_HIGHER - OMEGA_LOWER) / OMEGA_RESOLUTION;

    for (let i = 1; i <= ETA_RESOLUTION; i++) {
        let eta = i * ETA_STEP + ETA_LOWER;
        let etaPrev = (i - 1) * ETA_STEP + ETA_LOWER;

        for (let j = 0; j < OMEGA_RESOLUTION - 1; j++) {
            let omega = j * OMEGA_STEP + OMEGA_LOWER;
            let omegaNext = (j + 1) * OMEGA_STEP + OMEGA_LOWER;

            lowLeft = func(eta, omega);
            lowRight = func(eta, omegaNext);
            upRight = func(etaPrev, omegaNext);
            upLeft = func(etaPrev, omega);

            // Push triangles
            array.push(lowLeft);
            array.push(lowRight);
            array.push(upRight);
            array.push(lowLeft);
            array.push(upRight);
            array.push(upLeft);
        }

        // Connect last column to first column
        // Shift right to left for efficiency
        lowLeft = lowRight;
        upLeft = upRight;

        // Calculate first column
        lowRight = func(eta, OMEGA_LOWER);
        upRight = func(etaPrev, OMEGA_LOWER);

        // Push triangles
        array.push(lowLeft);
        array.push(lowRight);
        array.push(upRight);
        array.push(lowLeft);
        array.push(upRight);
        array.push(upLeft);
    }
}

/**
 * Parametric surface equation for supertoroids.
 * @param {number} eta
 * @param {number} omega
 */
function toroid_surface(eta, omega)
{
    let cosEta = Math.cos(eta);
    let sinEta = Math.sin(eta);
    let cosOmega = Math.cos(omega);
    let sinOmega = Math.sin(omega);
    let ALPHA_4 = RADIUS / Math.sqrt(ALPHA_1 * ALPHA_1 + ALPHA_2 * ALPHA_2);

    // Cache for efficiency
    let cached = ALPHA_4 + Math.sign(cosEta) * Math.pow(Math.abs(cosEta), EPSILON_1);

    return vec4(
        ALPHA_1 * cached * Math.sign(cosOmega) * Math.pow(Math.abs(cosOmega), EPSILON_2),
        ALPHA_2 * cached * Math.sign(sinOmega) * Math.pow(Math.abs(sinOmega), EPSILON_2),
        ALPHA_3 * Math.sign(sinEta) * Math.pow(Math.abs(sinEta), EPSILON_1),
        1.0
    );
}

/**
 * Parametric normal vector equation for toroids.
 * @param {number} eta 
 * @param {number} omega 
 */
function toroid_normal(eta, omega)
{
    let cosEta = Math.cos(eta);
    let sinEta = Math.sin(eta);
    let cosOmega = Math.cos(omega);
    let sinOmega = Math.sin(omega);

    // Cache for efficiency
    let cached = Math.sign(cosEta) * Math.pow(Math.abs(cosEta), 2 - EPSILON_1);

    return vec4(
        (1 / ALPHA_1) * cached * Math.sign(cosOmega) * Math.pow(Math.abs(cosOmega), 2 - EPSILON_2),
        (1 / ALPHA_2) * cached * Math.sign(sinOmega) * Math.pow(Math.abs(sinOmega), 2 - EPSILON_2),
        (1 / ALPHA_3) * Math.sign(sinEta) * Math.pow(Math.abs(sinEta), 2 - EPSILON_1),
        0.0
    );
}

function toroid_texture(eta, omega)
{
    // Limits
    const ETA_LOWER = -Math.PI;
    const ETA_HIGHER = Math.PI;
    const OMEGA_LOWER = -Math.PI;
    const OMEGA_HIGHER = Math.PI;

    // Step sizes
    const ETA_STEP = (ETA_HIGHER - ETA_LOWER) / ETA_RESOLUTION;
    const OMEGA_STEP = (OMEGA_HIGHER - OMEGA_LOWER) / OMEGA_RESOLUTION;

    return vec2((eta - ETA_LOWER) / ETA_STEP, (omega - OMEGA_LOWER) - OMEGA_STEP);
}

function toroid_vertices()
{
    toroid_generator(DATA_VERTICES, toroid_surface);
}

function toroid_normals()
{
    toroid_generator(DATA_NORMALS, toroid_normal);
}

function toroid_textures()
{
    toroid_generator(DATA_TEXTURE, toroid_texture)
}

/*
    LISTENERS
*/

function listener_update()
{
    // Listen to changes
    listener_shape();
    listener_shading();
    listener_mapping();

    // Finally, render the scene
    render();
}

// X rotation, did not change the name
function listener_theta()
{
    MV_THETA = document.getElementById("slider_theta").value;
    document.getElementById("val_theta").innerHTML = MV_THETA;
    MV_THETA = MV_THETA * Math.PI / 180;

    render();
}

// Y Rotation, did not change the name
function listener_gamma()
{
    // I know, gamma listener changes Phi while there is a phi listener
    MV_PHI = document.getElementById("slider_gamma").value;
    document.getElementById("val_gamma").innerHTML = MV_PHI;
    MV_PHI = MV_PHI * Math.PI / 180;

    render();
}

// Z Rotation, did not change the name
function listener_phi()
{
    let angle = document.getElementById("slider_phi").value;
    document.getElementById("val_phi").innerHTML = angle;
    angle = angle * Math.PI / 180;

    MV_UP = vec3(Math.cos(angle), Math.sin(angle), 0.0);

    render();
}

function listener_shape()
{
    var radios = document.getElementsByName("radio_shape");

    for (let i = 0; i < radios.length; i++) {
        if (radios[i].checked) {
            SELECTION_SHAPE = i;
            break;  // Only one can be checked
        }
    }

    render();
}

function listener_shading()
{
    var radios = document.getElementsByName("radio_shader");

    for (let i = 0; i < radios.length; i++) {
        if (radios[i].checked) {
            SELECTION_SHADER = i;
            break;  // Only one can be checked
        }
    }

    render();
}

function listener_mapping()
{
    var radios = document.getElementsByName("radio_mapping");

    for (let i = 0; i < radios.length; i++) {
        if (radios[i].checked) {
            SELECTION_MAPPING = i;            
            break; // Only one can be checked
        }
    }

    render();
}

function listener_eta()
{
    ETA_RESOLUTION = document.getElementById("slider_eta").value;
    document.getElementById("val_eta").innerHTML = ETA_RESOLUTION;

    render();
}

function listener_omega()
{
    OMEGA_RESOLUTION = document.getElementById("slider_omega").value;
    document.getElementById("val_omega").innerHTML = OMEGA_RESOLUTION;

    render();
}

function listener_epsilon1()
{
    EPSILON_1 = document.getElementById("slider_epsilon1").value;
    document.getElementById("val_epsilon1").innerHTML = EPSILON_1;

    render();
}

function listener_epsilon2()
{
    EPSILON_2 = document.getElementById("slider_epsilon2").value;
    document.getElementById("val_epsilon2").innerHTML = EPSILON_2;

    render();
}

function listener_alpha1()
{
    ALPHA_1 = document.getElementById("slider_alpha1").value;
    document.getElementById("val_alpha1").innerHTML = ALPHA_1;

    render();
}

function listener_alpha2()
{
    ALPHA_2 = document.getElementById("slider_alpha2").value;
    document.getElementById("val_alpha2").innerHTML = ALPHA_2;

    render();
}

function listener_alpha3()
{
    ALPHA_3 = document.getElementById("slider_alpha3").value;
    document.getElementById("val_alpha3").innerHTML = ALPHA_3;
    
    render();
}

function listener_radius()
{
    RADIUS = document.getElementById("slider_radius").value;
    document.getElementById("val_radius").innerHTML = RADIUS;

    render();
}
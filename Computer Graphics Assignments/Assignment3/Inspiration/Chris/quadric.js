//chris willette

var superquadric = {
  slices: 80,
  dices: 80,
  mBulge: 2,
  nBulge: 2,
  verts: null,
  normals: null,
  triangleStrip: null,
  texCoords: null,

  genVerts: function(v, u){
    var cosU = Math.cos(u);
    var sinU = Math.sin(u);
    var cosV = Math.cos(v);
    var sinV = Math.sin(v);
    var mB = this.mBulge;
    var nB = this.nBulge;
    var expo = function(arg, pow){
      if(arg === 0) return 0;
      return (arg * Math.pow(Math.abs(arg), pow - 1));
    };    
    var x = expo(cosV, 2 / mB) * expo(cosU, 2 / nB);
    var y = expo(cosV, 2 / mB) * expo(sinU, 2 / nB);
    var z = expo(sinV, 2 / mB);
    var verts = [x, y, z];
    var nx = expo(cosV, 2 - 2 / mB) * expo(cosU, 2 - 2 / nB);
    var ny = expo(cosV, 2 - 2 / mB) * expo(sinU, 2 - 2 / nB);
    var nz = expo(sinV, 2 - 2 / mB);
    var normals = norm3([nx, ny, nz]);
    return{verts : verts, normals : normals};
  },

  createGeometry: function() {
    var slices = this.slices;
    var dices = this.dices;
    var size = 3 * (dices + 1) * (slices + 1);
    this.verts = new Float32Array(size);
    var verts = this.verts;    
    this.normals = new Float32Array(size);
    var normals = this.normals;

    //this little block courtesy of 
    //https://ezekiel.encs.vancouver.wsu.edu/~cs442/lectures/shading/webgl-wireframe-shaded/Shapes.js
    var index = function(row, col) {
      return row * (dices + 1) + col;
    };
    for (var i = 0; i <= slices; i++) {
      var k = 3*index(0,i); // South Pole
      verts[k]     =  0;
      verts[k + 1] =  0;
      verts[k + 2] = -1;
      k = 3*index(dices,i); // North Pole
      verts[k]     =  0;
      verts[k + 1] =  0;
      verts[k + 2] = +1;
    };

    var dv = Math.PI / slices;
    var du = 2 * Math.PI / dices;
    for (var i = 1; i < slices; i++) {
      var v = -Math.PI / 2 + i * dv;
      for (var j = 0; j < dices; j++) {
        var u = -Math.PI + j * du;
        for (var k = 0; k < 3; k++) {
          var idx = 3 * index(i, j);
          verts[idx + k] = this.genVerts(v, u).verts[k];
          normals[idx + k] = this.genVerts(v, u).normals[k];
        };
      };
    };
    for (var i = 1; i < slices; i++) {
      var right = 3 * index(i, dices);
      var left = 3 * index(i, 0);
      for (var j = 0; j < 3; j++) {
        verts[right + j] = verts[left + j];
        normals[right + j] = normals[left + j];
      };
    };
  },


  //grabbed this bad boy straight from 
  //https://ezekiel.encs.vancouver.wsu.edu/~cs442/lectures/texture/torus/class/webgl-torus-mesh/torus.js
  createTriangleStrip: function() {
    var slices = this.slices;
    var dices = this.dices;
    var stripSize = slices * (2 * (dices + 1) + 2) - 2;
    this.triangleStrip = new Uint16Array(stripSize);
    var index = function(i, j) {
      return i * (dices + 1) + j;
    };
    var n = 0;
    for (var i = 0; i < slices; i++) {
      if (i > 0) {
        this.triangleStrip[n++] = index(i, 0);
      };
      for (var j = 0; j <= dices; j++) {
        this.triangleStrip[n++] = index(i + 1, j);
        this.triangleStrip[n++] = index(i, j);
      };
      if (i < slices - 1) {
        this.triangleStrip[n++] = index(i, dices);
      };
    };
  },


  createTexCoords: function() {
    var slices = this.slices;
    var dices = this.dices;
    var verts = this.verts;
    var size = 2 * (dices + 1) * (slices + 1);
    //2 * (M+1) * (N+1)
    //N slices = row #
    //M dices = col #
    //s = i/M
    //t = j/N
    //s0j = s1j
    //sNj = s(n-1)j
    //tiM = ti0
    this.texCoords = new Float32Array(size);
    var texCoords = this.texCoords;
    var index = function(row, col) {
      return (row * (dices + 1) + col);
    };
    var stIndex = function(row, col) {
      return 2 * index(row, col);
    };
    var vertTriple = function(row, col) {
      var idx = 3 * index(row, col);
      return [verts[idx], verts[idx + 1], verts[idx + 2]];
    };

    //eqn 15
    var distance = function(point, adjacent) {
      var sum = 0;
      for (var i = 0; i < 3; i++) {
        var diff = point[i] - adjacent[i];
        sum = sum + (diff * diff);
      };
      return Math.sqrt(sum);
    };

    //eqn 16

    //scale s-values
    for (var i = 1; i < slices; i++) {  
      var offset = 0;
      for (var j = 1; j <= dices; j++) {
        var point = vertTriple(i, j);
        var adjacent = vertTriple(i, j - 1);
        var dist = distance(point, adjacent);
        offset = offset + dist;
        texCoords[stIndex(i, j)] = offset;
      };
      for (var j = 1; j < dices; j++) {
        texCoords[stIndex(i, j)] = texCoords[stIndex(i, j)] / offset;
      };
      texCoords[stIndex(i, 0)] = 0;      
      texCoords[stIndex(i, dices)] = 1;
    };

    //copy the values from the adjacent lines of latitude
    //s0,j = s1,j and sN,j = sN,j for j = 1...M
    for (var j = 1; j <= dices; j++) {
      texCoords[stIndex(0, j)] = texCoords[stIndex(1, j)];
      texCoords[stIndex(slices, j)] = texCoords[stIndex(slices - 1, j)];
    };

    //and now repeat for t
    //took forever to remember that '+1'
    //is needed in texCoords[stIndex(i, j) + 1]
    //to constrain manipulations to  t-values
    for (var j = 0; j < dices; j++) {
      var offset = 0;
      for (var i = 1; i <= slices; i++) {
        var point = vertTriple(i, j);
        var adjacent = vertTriple(i - 1, j);
        var dist = distance(point, adjacent);
        offset = offset + dist;
        texCoords[stIndex(i, j) + 1] = offset;
      };
      for (var i = 1; i < slices; i++) {
        texCoords[stIndex(i, j) + 1] = texCoords[stIndex(i, j) + 1] / offset;
      };
      texCoords[stIndex(0, j) + 1] = 0;      
      texCoords[stIndex(slices, j) + 1] = 1;
    };
    //ti,M = ti,0 for i = 1...N
    for (var i = 1; i <= slices; i++) {
      texCoords[stIndex(i, dices) + 1] = texCoords[stIndex(i, 0) + 1];
    };
  }
};


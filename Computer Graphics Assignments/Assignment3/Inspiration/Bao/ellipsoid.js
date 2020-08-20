/**
 * Created by bao on 11/12/15.
 */

//create a superellipsoid object
var ellipsoid = {
    N: null,
    M: null,
    n: null,
    m: null,
    verts: null,
    normals: null,
    texCoords: null,
    longitude : null,
    latitude : null,
    //calculate normal and mesh vertices, store in arrays
    createMesh : function() {
        var N = this.N;
        var M = this.M;

        //budge factor
        var n = this.n;
        var m = this.m;
        var numFloats = 3*(N+1)*(M+1); //size of array

        //initialize arrays to hold vertices, normal and texture coordinates of the mesh
        if (!this.verts || this.verts.length != numFloats||!this.normals||this.normals.length != numFloats || !this.texCoords) {
            this.verts = new Float32Array(numFloats);
            this.normals = new Float32Array(numFloats);
            this.texCoords = new Float32Array(2*(N+1)*(M+1));
        }
        this.longitude = new Float32Array(N + 1); //slides
        this.latitude = new Float32Array(M + 1); //dices

        //var l = 0;
        //for each slice from 0 to N
        for (var i = 0; i <= N; i++) {
            //for each dices from 0 to M
            //var s = i/N;
            for (var j = 0; j <= M; j++) {
                //var t =  j/M;
                var u = (2 * Math.PI / M) * j - Math.PI;
                var v = (Math.PI / N) * i - Math.PI / 2;

                //store each x,y,z for each vertices
                this.verts[3 * (i * (M + 1) + j)] = x_u_v(u,v,n,m);
                this.verts[3 * (i * (M + 1) + j) + 1] = y_u_v(u,v,n,m);
                this.verts[3 * (i * (M + 1) + j) + 2] = z_u_v(v,m);

                this.normals[3 * (i * (M + 1) + j)] = n_x(u,v,n,m);
                this.normals[3 * (i * (M + 1) + j) + 1] = n_y(u,v,n,m);
                this.normals[3 * (i * (M + 1) + j) + 2] = n_z(v,m);

                // this.texCoords[l] = s;
                //this.texCoords[l+1] = t;
                // l += 2;

                // store distances between successive vertices in arrays
                var x1, x2, y1, y2, z1, z2, d;
                if(i > 0){ //start to calculate distance between points along slices
                    x1 = this.verts[3 * ((i - 1) * (M + 1) + j)];
                    y1 = this.verts[3 * ((i - 1) * (M + 1) + j) + 1];
                    z1 = this.verts[3 * ((i - 1) * (M + 1) + j) + 2];
                    x2 = this.verts[3 * (i * (M + 1) + j)];
                    y2 = this.verts[3 * (i * (M + 1) + j) + 1];
                    z2 = this.verts[3 * (i * (M + 1) + j) + 2];
                    d = Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
                    this.latitude[j] += d; //summation d_ik( k 1 to M)
                }

                if(j > 0){ //along dices
                    x1 = this.verts[3 * (i * (M + 1) + j - 1)];
                    y1 = this.verts[3 * (i * (M + 1) + j - 1) + 1];
                    z1 = this.verts[3 * (i * (M + 1) + j - 1) + 2];
                    x2 = this.verts[3 * (i * (M + 1) + j)];
                    y2 = this.verts[3 * (i * (M + 1) + j) + 1];
                    z2 = this.verts[3 * (i * (M + 1) + j) + 2];
                    d = Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
                    this.longitude[i] += d; //summation d_ik (k 1 to N)
                }
            }
        }
        //store value of s and t in textCoor arrays
        for(var i = 0; i <= N ; i++){
            var d = 0;
            for(var j = 0; j <= M ; j++){
                //s(i,0) = 0
                if(j == 0) {
                    this.texCoords[2 * (i * (M + 1) + j)] = 0;
                    //otherwise j
                }else{
                    x1 = this.verts[3 * (i * (M + 1) + j - 1)];
                    y1 = this.verts[3 * (i * (M + 1) + j - 1) + 1];
                    z1 = this.verts[3 * (i * (M + 1) + j - 1) + 2];
                    x2 = this.verts[3 * (i * (M + 1) + j)];
                    y2 = this.verts[3 * (i * (M + 1) + j) + 1];
                    z2 = this.verts[3 * (i * (M + 1) + j) + 2];
                    //d_ik ( k from 1 to j)
                    d += Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
                    //s = dik( 1 to j)/ dik(1 to M)
                    this.texCoords[2 * (i * (M + 1) + j)] = d/this.longitude[i];
                }
            }
        }

        for(var j = 0; j < M + 1; j++){
            var d = 0;
            for(var i = 0; i < N + 1; i++){
                //t(j,0) = t(j,1)
                if(i == 0){
                    this.texCoords[2*(i*(M+1)+j)+1] = this.texCoords[2*((M+1)+j)+1];
                }else if(j == M){ //duplicate these values where the mesh wraps around on itself
                    this.texCoords[2*(i*(M+1)+j)+1] = this.texCoords[2*(i*(M+1))+1];
                } else{
                    x1 = this.verts[3 * ((i - 1) * (M + 1) + j)];
                    y1 = this.verts[3 * ((i - 1) * (M + 1) + j) + 1];
                    z1 = this.verts[3 * ((i - 1) * (M + 1) + j) + 2];
                    x2 = this.verts[3 * (i * (M + 1) + j)];
                    y2 = this.verts[3 * (i * (M + 1) + j) + 1];
                    z2 = this.verts[3 * (i * (M + 1) + j) + 2];
                    d += Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
                    this.texCoords[2 * (i * (M + 1) + j) + 1] = d/this.latitude[j];
                }
            }
        }
        //s(0,j) = s(1,j) and s(N,j) = s(N -1, j) at poles
        for(var j = 0; j <= M; j++){
            this.texCoords[2 * j] = this.texCoords[2 * ((M + 1) + j)];
            this.texCoords[2 * (N * (M + 1) + j)] = this.texCoords[2 * ((N - 1) * (M + 1) + j)];
        }

    },

    //generate triangle strips and store in array
    triangleStrip: null,
    createTriangleStrip : function() {
        var M = this.M, N = this.N;
        var numIndices = N*(2*(M + 1) + 2) - 2;
        if (!this.triangleStrip || this.triangleStrip.length != numIndices)
            this.triangleStrip = new Uint16Array(numIndices);
        var index = function(i, j) {
            return i*(M+1) + j;
        };
        var n = 0;
        for (var i = 0; i < N; i++) {
            if (i > 0)  // degenerate  index
                this.triangleStrip[n++] = index(i,0);
            for (var j = 0; j <= M; j++) {
                this.triangleStrip[n++] = index(i+1,j);
                this.triangleStrip[n++] = index(i,j);
            }
            if (i < N-1) // degenerate index
                this.triangleStrip[n++] = index(i,M)
        }
    },

    //create hedgehog
    createHedgeHog : function() {
        var hedgehog = [];
        var hedgeHogLength = 0.2;
        for (var i = 0; i < this.normals.length; i += 3) {
            var p = [this.verts[i], this.verts[i+1], this.verts[i+2]];
            var n = [this.normals[i], this.normals[i+1], this.normals[i+2]];
            var q = [p[0] + hedgeHogLength*n[0],
                p[1] + hedgeHogLength*n[1],
                p[2] + hedgeHogLength*n[2]];
            hedgehog.push(p[0], p[1], p[2],
                q[0], q[1], q[2]);
        }
        this.hedgeHog = new Float32Array(hedgehog);
    },

    //create wireframe of the mesh
    createWireFrame : function() {
        var lines = [];
        lines.push(this.triangleStrip[0], this.triangleStrip[1]);
        var numStripIndices = this.triangleStrip.length;
        for (var i = 2; i < numStripIndices; i++) {
            var a = this.triangleStrip[i-2];
            var b = this.triangleStrip[i-1];
            var c = this.triangleStrip[i];
            if (a != b && b != c && c != a)
                lines.push(a, c, b, c);
        }
        this.wireframe = new Uint16Array(lines);
        var l = lines.length;
    }
};

//functions helps to build the coordinates of vertices of mesh
function x_u_v(u,v,n,m){
    if(Math.abs(Math.cos(v)) < 0.000000005 || Math.abs(Math.cos(u)) < 0.000000005 ){
        return 0;
    }else
        return  Math.cos(v) * Math.pow(Math.abs(Math.cos(v)),(2.0/m) - 1) * Math.cos(u) * Math.pow(Math.abs(Math.cos(u)),(2.0/n) - 1);
}
function y_u_v(u,v,n,m){
    if(Math.abs(Math.cos(v)) < 0.000000005 || Math.abs(Math.sin(u)) < 0.00000005){
        return 0;
    }else
        return  Math.cos(v) * Math.pow(Math.abs(Math.cos(v)),(2.0/m) - 1) * Math.sin(u) * Math.pow(Math.abs(Math.sin(u)),(2.0/n) - 1);
}
function z_u_v(v,m) {
    if(Math.abs(Math.sin(v)) < 0.000000005){
        return 0;
    }else
        return Math.sin(v) * Math.pow(Math.abs(Math.sin(v)),(2.0/m) - 1);
}

//normal coordinates
function n_x(u,v,n,m){
    if(Math.abs(Math.cos(v)) < 0.000000005 || Math.abs(Math.cos(u)) < 0.000000005 ){
        return 0;
    }else
        return  Math.cos(v) * Math.pow(Math.abs(Math.cos(v)),(1 - 2.0/m)) * Math.cos(u) * Math.pow(Math.abs(Math.cos(u)),(1 - 2.0/n));
}

function n_y(u,v,n,m){
    if(Math.abs(Math.cos(v)) < 0.000000005 || Math.abs(Math.sin(u)) < 0.00000005){
        return 0;
    }else
        return  Math.cos(v) * Math.pow(Math.abs(Math.cos(v)),(1 - 2.0/m)) * Math.sin(u) * Math.pow(Math.abs(Math.sin(u)),(1 -  2/n));
}

function n_z(v,m){
    if(Math.abs(Math.sin(v)) < 0.000000005){
        return 0;
    }else
        return Math.sin(v) * Math.pow(Math.abs(Math.sin(v)), 1 - 2.0/m);
}







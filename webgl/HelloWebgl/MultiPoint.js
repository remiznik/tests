// HelloPoint1.js (c) 2012 matsuda
// Vertex shader program
var VSHADER_SOURCE = 
  // x' = x * cos(b) - y * sin(b)
  // y' = x * sin(b) + y * cos(b)
  'attribute vec4 a_Position;\n' +   
  'uniform vec4 u_Translation;\n' + 
  'uniform mat4 u_ModelMatrix;\n' +
  'void main() {\n' +
  ' gl_Position = u_ModelMatrix * a_Position;\n' +
  '}\n';

var FSHADER_SOURCE =
  'precision mediump float;\n' +
  'uniform vec4 u_FragColor;\n' + // uniform-переменная <- (1)
  'void main() {\n' +
  // ' gl_FragColor = u_FragColor;\n' +
  ' gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n' +
  '}\n';

var Tx = 0.5, Ty = 0.5, Tz = 0.0;
var ANGLE = 60.0;
var ANGLE_STEP = 25.0;

function main() {
  var canvas = document.getElementById('webgl');

  // Get the rendering context for WebGL
  var gl = getWebGLContext(canvas);
  if (!gl) {
    console.log('Failed to get the rendering context for WebGL');
    return;
  }

  // Initialize shaders
  if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
    console.log('Failed to intialize shaders.');
    return;
  }

  var n = initVertexBuffers(gl);
  if (n < 0) {
    console.log('Failed to set te positions to the vertives');
    return;
  }

  gl.clearColor(0.0, 0.0, 0.0, 1.0);

  var u_Translation = gl.getUniformLocation(gl.program, 'u_Translation');
  if (u_Translation < 0){
    console.log('Failde to get the storage location of u_Translation');
    return;
  }
  gl.uniform4f(u_Translation, Tx, Ty, Tz, 0.0);

  var currentAngle = 0.0;
  var tr = 0.0;
  var modelMatrix = new Matrix4();

  var u_ModelMatrix = gl.getUniformLocation(gl.program, 'u_ModelMatrix');
  var tick = function() {
    currentAngle = animate(currentAngle);
    tr = translate(tr);
    draw(gl, n, currentAngle, tr, modelMatrix, u_ModelMatrix);
    requestAnimationFrame(tick);
  }
  tick();
}


function initVertexBuffers(gl) {
  var vertices = new Float32Array([0.0, 0.5, -0.5, -0.5, 0.5, -0.5]);
  var n = 3;

  var vertexBuffer = gl.createBuffer();
  if (!vertexBuffer) {
    console.log('Failed to create the buffer object');
    return;
  }

  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

  var a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  if (a_Position < 0)  {
    console.log('Failed to get the storage location of a_Position');
    return;
  }

  gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(a_Position);

  return n;
}

var g_points = [];
var g_colors = [];
function click(ev, gl, canvas, a_Position, u_FragColor) {
  var x = ev.clientX;
  var y = ev.clientY;
  var rect = ev.target.getBoundingClientRect();

  x = ((x - rect.left) - canvas.width / 2) / (canvas.width / 2);
  y = (canvas.height / 2 - (y - rect.top)) / (canvas.height / 2);
  g_points.push([x,y]);

  if (x >= 0.0 && y >= 0.0) {
    g_colors.push([1.0, 0.0, 0.0, 1.0]);
  } else if (x < 0.0 && y < 0.0) {
    g_colors.push([0.0, 1.0, 0.0, 1.0]);
  } else {
    g_colors.push([1.0, 1.0, 1.0, 1.0]);
  }

  gl.clear(gl.COLOR_BUFFER_BIT);

  // gl.vertexAttrib3f(a_Position, x, y, 0.0);
  // gl.drawArrays(gl.POINTS, 0, 1);

  var len = g_points.length;
  for (var i = 0; i < len; i += 1) {
    var rgba = g_colors[i];
    gl.vertexAttrib3f(a_Position, g_points[i][0], g_points[i][1], 0.0);
    gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
    gl.drawArrays(gl.POINTS, 0, 1);
  }
}

var rev = 0;
function translate(tr) {
  if (tr >= 1.0) {
    rev = 1;
  } 
  if (tr <= -1)
  {
    rev = 0;
  }

  if (rev == 0) {
    tr += 0.05;
  } else {
    tr -= 0.05;
  }

  return tr;
}
function draw(gl, n, currentAngle, tr, modelMatrix, u_ModelMatrix) {
  modelMatrix.setRotate(currentAngle, 0, 0, 1);
  modelMatrix.translate(tr, 0, 0);

  gl.uniformMatrix4fv(u_ModelMatrix, false, modelMatrix.elements);

  gl.clear(gl.COLOR_BUFFER_BIT);

  gl.drawArrays(gl.TRIANGLES, 0, n);
}

var g_last = Date.now();
function animate(angle) {
  var now = Date.now();
  var elapsed = now - g_last;
  g_last = now;

  var newAngle = angle + (ANGLE_STEP * elapsed) / 1000.0;
  return newAngle %= 360;
}

function dbclick(ev, gl, canvas, a_Position) {
  gl.clear(gl.COLOR_BUFFER_BIT);
  var len = g_points.length;
  for (var i = 0; i < len; i += 2) {
    gl.vertexAttrib3f(a_Position, g_points[i][0], g_points[i][1], 0.0);

    gl.drawArrays(gl.POINTS, 0, 1);
  } 
}

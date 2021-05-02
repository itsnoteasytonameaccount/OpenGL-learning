
{
  if (typeof exports === "undefined") {
    window.exports = {
      "gl-matrix": glMatrix
    };
  }
  if (typeof module === "undefined") {
    window.module = {
      exports
    };
  }
  if (typeof require === "undefined") {
    window.require = function require (name) {
      return exports[name];
    };
  }
}
import MainLoop from "./MainLoop";

const instance = MainLoop.getInstance();
instance.mount("#container");
let count = 0;
// Draw the scene repeatedly
function render() {
    instance.loop();
    requestAnimationFrame(render);
}
requestAnimationFrame(render);

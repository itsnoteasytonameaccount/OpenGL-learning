import MainLoop from "./MainLoop";

const instance = MainLoop.getInstance();
instance.mount("#container");
// Draw the scene repeatedly
function render() {
    instance.loop();
    requestAnimationFrame(render);
}
requestAnimationFrame(render);

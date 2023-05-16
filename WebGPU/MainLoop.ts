import FirstScene from "./scenes/FirstScene";
import SceneBase from "./scenes/SceneBase";

export default class MainLoop {
    cvs: HTMLCanvasElement;
    ctx: GPUCanvasContext = null;
    device: GPUDevice = null;
    scene: SceneBase;
    static instance: MainLoop = null;

    private constructor() {
        this.cvs = document.createElement("canvas");
        this.cvs.height = 1000;
        this.cvs.width = 1000;
        this.init();
    }

    async init() {
        if (!navigator.gpu) {
            throw Error("WebGPU not supported.");
        }
        const adapter = await navigator.gpu.requestAdapter();
        if (!adapter) {
            throw Error("Couldn't request WebGPU adapter.");
        }
        const device = await adapter.requestDevice();
        let ctx: GPUCanvasContext = this.cvs.getContext("webgpu");
        ctx.configure({
            device,
            format: navigator.gpu.getPreferredCanvasFormat(),
            alphaMode: "premultiplied",
        });
        this.ctx = ctx;
        this.device = device;
        this.scene = new FirstScene(ctx, device);
    }

    mount(query: string): void {
        const node = document.querySelector(query);
        if (node === null) {
            return;
        }
        this.mountInNode(node);
    }

    mountInNode(node: Element) {
        node.appendChild(this.cvs);
    }

    loop() {
        if (this.scene) {
            this.scene.draw();
        }
    }

    static getInstance() {
        if (this.instance === null) {
            this.instance = new MainLoop();
        }
        return this.instance;
    }
}

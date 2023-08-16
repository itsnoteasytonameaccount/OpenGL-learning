import PhoneLightScene from "./scenes/PhoneLightScene";
import SceneBase from "./scenes/SceneBase";

export default class MainLoop {
    cvs: HTMLCanvasElement;
    ctx: WebGLContext;
    scene: SceneBase;
    static instance: MainLoop = null;

    private constructor() {
        this.cvs = document.createElement("canvas");
        this.cvs.height = 1000;
        this.cvs.width = 1000;
        let ctx: WebGLContext | null = this.cvs.getContext("webgl2");
        if (ctx === null) {
            console.log("webgl version1");
            ctx = this.cvs.getContext("webgl");
        }
        if (ctx === null) {
            throw new Error("Not support webGL!");
        }
        this.ctx = ctx;
        this.scene = new PhoneLightScene(ctx);
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
        this.scene.draw();
    }

    static getInstance() {
        if (this.instance === null) {
            this.instance = new MainLoop();
        }
        return this.instance;
    }
}

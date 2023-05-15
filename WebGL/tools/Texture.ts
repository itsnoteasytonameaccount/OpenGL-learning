export default class Texture {
    static async load2DTexture(textureId: WebGLTextureId, image: HTMLImageElement, gl: WebGLContext) {
        const texture = gl.createTexture();
        gl.activeTexture(textureId);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.MIRRORED_REPEAT);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.MIRRORED_REPEAT);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.generateMipmap(gl.TEXTURE_2D);
    }

    static loadImage(path: string, clip: boolean = false): Promise<HTMLImageElement> {
        const image = new Image();
        return new Promise((resolve) => {
            image.onload = () => {
                if (clip) {
                    this.handleImageLoad(image, resolve);
                } else {
                    resolve(image);
                }
            };
            image.src = path;
        });
    }

    static handleImageLoad(image: HTMLImageElement, callback: Function): void {
        let width = 2;
        let height = 2;
        console.log("image", image.height, image.width);
        while (width <= image.width) width *= 2;
        while (height <= image.height) height *= 2;

        // 纹理的宽高值需要等于2的n次幂
        this.imageClip(image, width, height).then((image) => {
            console.log("image loaded!");
            callback(image);
        });
    }

    static imageClip(image: HTMLImageElement, width: number, height: number): Promise<HTMLImageElement> {
        if (image.height === height && image.width === width) {
            return Promise.resolve(image);
        }
        const canvas: HTMLCanvasElement = document.createElement("canvas");
        Object.assign(canvas, {
            width,
            height,
        });
        const ctx: CanvasRenderingContext2D | null = canvas.getContext("2d");
        if (!ctx) {
            Promise.reject(new Error("failed to create 2d ctx!"));
        }
        ctx.drawImage(image, 0, 0);
        console.log("image cliped", height, width);
        return new Promise((resolve) => {
            const clipedImage = new Image();
            clipedImage.onload = () => resolve(clipedImage);
            clipedImage.src = canvas.toDataURL("image/png");
        });
    }
}

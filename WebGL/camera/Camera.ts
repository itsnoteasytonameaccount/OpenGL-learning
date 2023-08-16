import { glMatrix, mat4, vec3 } from "gl-matrix";

export default class Camera {
    position: vec3;
    front: vec3;
    right: vec3;
    up: vec3;

    // 偏航角
    yaw: number;

    // 俯仰角，仰视为正
    // y = -sin(pitch)
    // x = cos(pitch) * -sin(yaw)
    // z = cos(pitch) * cos(yaw)
    pitch: number;

    constructor(...args: [vec3, number, number] | [vec3]) {
        if (args.length === 1) {
            this.init(...args);
        } else {
            this.init2(...args);
        }
    }

    init(position: vec3) {
        this.position = vec3.clone(position);
        this.front = vec3.create();
        vec3.normalize(this.front, this.position);
        this.updateAngle();
    }

    init2(position: vec3, yaw: number, pitch: number) {
        this.position = vec3.clone(position);
        this.yaw = yaw;
        this.pitch = pitch;
        this.updateCameraVectors();
    }

    updatePosition(position: vec3) {
        this.position = vec3.clone(position);
    }

    updateAngle(): void {
        const pitchRadian = -Math.asin(this.front[1]);
        const cosPitch = Math.cos(pitchRadian);
        const yawRadian = Math.acos(this.front[2] / cosPitch);

        this.pitch = Camera.radianToAngle(pitchRadian);
        this.yaw = Camera.radianToAngle(yawRadian);
        this.updateAxis();
    }

    updateCameraVectors(): void {
        const x = -Math.cos(glMatrix.toRadian(this.pitch)) * Math.sin(glMatrix.toRadian(this.yaw));
        const z = Math.cos(glMatrix.toRadian(this.pitch)) * Math.cos(glMatrix.toRadian(this.yaw));
        const y = -Math.sin(glMatrix.toRadian(this.pitch));
        this.front = vec3.create();
        vec3.normalize(this.front, vec3.fromValues(x, y, z));
        this.updateAxis();
    }

    updateAxis(): void {
        const world_up = vec3.fromValues(0, 1.0, 0);
        const negFront = vec3.negate(vec3.create(), this.front);
        this.right = vec3.create();
        this.up = vec3.create();
        vec3.normalize(this.right, vec3.cross(this.right, negFront, world_up));
        vec3.normalize(this.up, vec3.cross(this.up, this.front, this.right));
    }

    getViewMatrix(): mat4 {
        const lookAtPos = vec3.create();
        vec3.add(lookAtPos, this.position, vec3.negate(lookAtPos, this.front));
        return Camera.lookAt(this.position, lookAtPos, this.up);
    }

    setYawPitch(yaw: number, pitch: number) {
        this.yaw = yaw;
        this.pitch = pitch;
        this.updateCameraVectors();
    }

    static lookAt(position: vec3, lookAtPos: vec3, up: vec3): mat4 {
        return mat4.lookAt(mat4.create(), position, lookAtPos, up);
    }

    static radianToAngle(radian: number): number {
        return Math.floor((360 / Math.PI) * radian * 100) / 100;
    }
}

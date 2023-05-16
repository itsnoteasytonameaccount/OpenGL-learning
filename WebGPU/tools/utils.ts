export function throttle(fn: Function, duration: number = 500): Function {
    let time = Date.now();
    let used = false;
    return function (...args) {
        const now = Date.now();
        if (now - time > duration) {
            time = now;
            used = false;
        }
        if (used == true) {
            return;
        }
        used = true;
        fn.apply(this, args);
    };
}

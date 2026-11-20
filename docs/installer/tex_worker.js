import { buildTextureDdsBytes } from "./tex_convert.js?v=20261118f";

self.onmessage = async (e) => {
  const { id, job } = e.data;
  try {
    const bytes = await buildTextureDdsBytes(job);
    self.postMessage({ id, bytes }, [bytes.buffer]);
  } catch (err) {
    self.postMessage({ id, error: err && err.message ? err.message : String(err) });
  }
};

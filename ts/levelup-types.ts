// levelup types for Typescript convenience.

export interface batchOperation {
  type: string,
  key: string|Uint8Array,
  value: string|Uint8Array
}

/** @type {import('ts-jest').JestConfigWithTsJest} */
module.exports = {
  preset: 'ts-jest',
  testEnvironment: 'node',
  testPathIgnorePatterns: [
    "/node_modules/"
  ],
  slowTestThreshold: 5,
  transform: {
    "jack$": "jest-text-transformer"
  }
};
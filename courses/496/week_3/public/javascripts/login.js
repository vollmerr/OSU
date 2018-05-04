const baseUrl = `https://accounts.google.com/o/oauth2/v2/auth`;
const responseType = `code`;
const clientId = `657916121904-fu72jjchacfmh2lfn2poumbj3tov8ib9.apps.googleusercontent.com`;
const redirectUri = `${location.origin}`;
const scope = `email`;
const state = uuidv4();

const url = `${baseUrl}?response_type=${responseType}&client_id=${clientId}&redirect_uri=${redirectUri}&scope=${scope}&state=${state}`;

fetch(url);
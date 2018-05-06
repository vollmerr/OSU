require('dotenv').config();
const fetch = require('node-fetch');
const formurlencoded = require('form-urlencoded');
const uuid = require('uuid/v4');

const { auth } = require('./config');


const Auth = () => {
    const states = {};
    
    const setState = () => {
        const id = uuid();
        states[id] = 1;
        return id;
    };

    const getState = (x) => states[x];

    const getAuthUrl = () => `${auth.urls.auth}`
        + `?response_type=${auth.response}`
        + `&client_id=${auth.client.id}`
        + `&redirect_uri=${auth.urls.redirect}`
        + `&scope=${auth.scope}`
        + `&state=${setState()}`;

    const getTokenUrl = ({ code }) => `${auth.urls.token}`
        + `?code=${code}`
        + `&client_id=${auth.client.id}`
        + `&client_secret=${auth.client.secret}`
        + `&redirect_uri=${auth.urls.redirect}`
        + `&grant_type=${auth.grantType}`

    const getToken = async ({ code }) => {
        const url = getTokenUrl({ code });
        const options = {
            method: 'POST',
        };
        
        try {
            const res = await fetch(url, options);
            return res.json();
        } catch (err) {
            throw err;
        }
    }

    return Object.freeze({
        setState,
        getState,
        getAuthUrl,
        getToken,
    });
};


module.exports = Auth;

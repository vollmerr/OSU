require('dotenv').config();
const fetch = require('node-fetch');
const formurlencoded = require('form-urlencoded');
const uuid = require('uuid/v4');

const ds = require('./connect');
const { auth } = require('./config');


const Auth = () => {
    const setState = async () => {
        const state = uuid();
        const key = ds.key(['state', state]);
        await ds.insert({ data: 1, key });
        return state;
    };

    const getState = async ({ state }) => {
        const key = ds.key(['state', state]);
        return ds.get(key);
    };

    const getAuthUrl = async () => `${auth.urls.auth}`
        + `?response_type=${auth.response}`
        + `&client_id=${auth.client.id}`
        + `&redirect_uri=${auth.urls.redirect}`
        + `&scope=${auth.scope}`
        + `&state=${await setState()}`;

    const getTokenUrl = ({ code }) => `${auth.urls.token}`
        + `?code=${code}`
        + `&client_id=${auth.client.id}`
        + `&client_secret=${auth.client.secret}`
        + `&redirect_uri=${auth.urls.redirect}`
        + `&grant_type=${auth.grantType}`;

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

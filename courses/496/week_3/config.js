module.exports = {
    auth: {
        client: {
            id: process.env.OAUTH_CLIENT_ID,
            secret: process.env.OAUTH_CLIENT_SECRET,
        },
        urls: {
            auth: `https://accounts.google.com/o/oauth2/v2/auth`,
            token: `https://www.googleapis.com/oauth2/v4/token`,
            redirect: `${process.env.APP_URL}/redirect`,
        },
        scope: `email`,
        response: `code`,
        grantType: `authorization_code`,
    },
};
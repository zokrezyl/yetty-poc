/**
 * Cloudflare Worker to proxy vfsync.org with CORS headers
 *
 * Deploy this to Cloudflare Workers (free tier: 100k requests/day)
 * Then use the worker URL in your JSLinux config
 *
 * To deploy:
 * 1. Create account at https://dash.cloudflare.com
 * 2. Go to Workers & Pages -> Create Worker
 * 3. Paste this code
 * 4. Deploy
 * 5. Use the worker URL (e.g., https://vfsync-proxy.yourname.workers.dev)
 */

export default {
  async fetch(request, env, ctx) {
    const url = new URL(request.url);

    // Handle CORS preflight
    if (request.method === 'OPTIONS') {
      return new Response(null, {
        headers: {
          'Access-Control-Allow-Origin': '*',
          'Access-Control-Allow-Methods': 'GET, OPTIONS',
          'Access-Control-Allow-Headers': 'Content-Type',
          'Access-Control-Max-Age': '86400',
        },
      });
    }

    // Only allow GET requests
    if (request.method !== 'GET') {
      return new Response('Method not allowed', { status: 405 });
    }

    // Proxy to vfsync.org
    const vfsyncUrl = `https://vfsync.org${url.pathname}${url.search}`;

    try {
      const response = await fetch(vfsyncUrl, {
        headers: {
          'User-Agent': 'Mozilla/5.0',
          'Accept': '*/*',
          'Origin': 'https://bellard.org',
          'Referer': 'https://bellard.org/jslinux/',
        },
      });

      // Clone the response and add CORS headers
      const newHeaders = new Headers(response.headers);
      newHeaders.set('Access-Control-Allow-Origin', '*');
      newHeaders.set('Access-Control-Allow-Methods', 'GET, OPTIONS');
      newHeaders.delete('Content-Security-Policy');
      newHeaders.delete('X-Frame-Options');

      return new Response(response.body, {
        status: response.status,
        statusText: response.statusText,
        headers: newHeaders,
      });
    } catch (error) {
      return new Response(`Proxy error: ${error.message}`, { status: 502 });
    }
  },
};

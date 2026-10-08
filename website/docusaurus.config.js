const repo = 'https://github.com/martoney/integrations-test';

/** @type {import('@docusaurus/types').Config} */
module.exports = {
  title: 'PLC Dummy SDK',
  tagline: 'Infrastructure test bed for the PLC-36 SDK',
  url: 'https://martoney.github.io',
  baseUrl: '/integrations-test/',
  onBrokenLinks: 'throw',
  markdown: {
    format: 'detect',
    hooks: {onBrokenMarkdownLinks: 'throw'},
  },
  presets: [
    [
      'classic',
      {
        docs: {
          path: '../docs',
          routeBasePath: '/',
          editUrl: ({docPath}) => `${repo}/edit/main/docs/${docPath}`,
        },
        blog: false,
      },
    ],
  ],
  themeConfig: {
    navbar: {
      title: 'PLC Dummy SDK',
      items: [{href: repo, label: 'GitHub', position: 'right'}],
    },
  },
};

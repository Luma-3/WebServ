hljs.registerLanguage('configlang', function(hljs) {
	return {
		name: 'ConfigLang',
		keywords: {
			keyword: 'listen root index hostname server autoindex error_page location method cgi max_body_size log',
			literal: 'on off php py DELETE GET POST Info Debug Error Warnnig',
		},
		contains: [
			hljs.COMMENT('#', '$'),
			{
				claassName: 'string',
				begin: '"', end: '"'
			},
			{
				className: 'number',
				begin: '[0-9]+'
			}
		]
	}
});
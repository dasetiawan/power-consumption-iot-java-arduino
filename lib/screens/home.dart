import 'package:flutter/material.dart';
import 'web_view_container.dart';

class Home extends StatelessWidget {
  static final links = [
    'https://demo.thingsboard.io/dashboard/36f6caa0-7a70-11e9-a31b-bbaebe3575d3?publicId=8fa7d510-7a64-11e9-a31b-bbaebe3575d3'
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: Text("Monitor App"),
        ),
        key: Key('123'),
        body: SafeArea(
            child: SingleChildScrollView(
                child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: links.map((link) => _urlButton(context, link)).toList(),
        ))));
  }

  Widget _urlButton(BuildContext context, String url) {
    return Container(
        padding: EdgeInsets.all(20.0),
        child: FlatButton(
          color: Theme.of(context).primaryColor,
          padding: const EdgeInsets.symmetric(horizontal: 24.0, vertical: 15.0),
          child: Text("Electric Power Consumption", style: TextStyle(color: Colors.white),),
          onPressed: () => _handleURLButtonPress(context, url),
        ));
  }

  void _handleURLButtonPress(BuildContext context, String url) {
    Navigator.push(context,
        MaterialPageRoute(builder: (context) => WebViewContainer(url)));
  }
}

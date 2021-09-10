//
//  ContentView.swift
//  pipes_gui
//
//  Created by admin on 10.09.2021.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        Text(PipesTestWrapper().pipesTest_run()).frame(width: 400, height: 400, alignment:.topLeading).padding(10)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

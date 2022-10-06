use std::{
    alloc::{alloc, Layout},
    mem::{self, size_of},
    ptr,
    sync::mpsc::Receiver,
    thread, time,
};

use glutin::{self, Api, GlContext, GlProfile, GlRequest};

mod texture;
use texture::Texture;

mod shader;
use shader::Shader;

pub struct Viewport {
    window: tauri::Window,
    width: u32,
    height: u32,
    shader: Shader,
    texture: Texture,
    vao: u32,
    buffer: *mut u8,
}

impl Viewport {
    pub fn new(window: tauri::Window, width: u32, height: u32) -> Self {
        let gl_version = GlRequest::Specific(Api::OpenGl, (3, 3));
        let gl_profile = GlProfile::Core;
        let context = glutin::HeadlessRendererBuilder::new(width, height)
            .build()
            .unwrap();
        unsafe {
            context.make_current().unwrap();
        }

        gl::load_with(|symbol| context.get_proc_address(symbol) as *const _);

        unsafe {
            gl::Viewport(0, 0, width as i32, height as i32);
        }
        let shader = Shader::new();
        let texture = Texture::new(width, height);

        let vertices = [
            -0.5f32, -0.5f32, 0.0f32, 0.5f32, -0.5f32, 0.0f32, 0.0f32, 0.5f32, 0.0f32,
        ];
        let mut vbo: u32 = 0;
        let mut vao: u32 = 0;
        unsafe {
            gl::GenVertexArrays(1, &mut vao);
            gl::BindVertexArray(vao);

            gl::GenBuffers(1, &mut vbo);
            gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
            gl::BufferData(
                gl::ARRAY_BUFFER,
                (vertices.len() * size_of::<f32>()) as isize,
                mem::transmute(vertices.as_ptr()),
                gl::STATIC_DRAW,
            );

            gl::VertexAttribPointer(
                0,
                3,
                gl::FLOAT,
                gl::FALSE,
                (3 * size_of::<f32>()) as i32,
                ptr::null(),
            );
            gl::EnableVertexAttribArray(0);
        }
        println!("Vieport successfully loaded");
        let cap = (width * height) as usize;
        let layout = Layout::from_size_align(cap, mem::align_of::<u8>()).expect("Bad layout");
        let buffer: *mut u8;
        unsafe { buffer = alloc(layout) }

        Self {
            window,
            width,
            height,
            shader,
            texture,
            vao,
            buffer,
        }
    }
    pub fn run(&self, rx: Receiver<()>) {
        loop {
            self.draw();
            if rx.try_recv().is_ok() {
                break;
            }
            thread::sleep(time::Duration::from_millis(16));
        }
    }

    fn draw(&self) {
        self.shader.use_program();
        self.texture.bind();
        unsafe {
            gl::BindVertexArray(self.vao);
            gl::DrawArrays(gl::TRIANGLES, 0, 3);
            gl::ReadPixels(
                0,
                0,
                self.width as i32,
                self.height as i32,
                gl::RGB,
                gl::UNSIGNED_BYTE,
                mem::transmute(self.buffer),
            );
        }
    }
}

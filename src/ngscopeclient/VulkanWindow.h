/***********************************************************************************************************************
*                                                                                                                      *
* glscopeclient                                                                                                        *
*                                                                                                                      *
* Copyright (c) 2012-2022 Andrew D. Zonenberg                                                                          *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

/**
	@file
	@author Andrew D. Zonenberg
	@brief Declaration of VulkanWindow
 */
#ifndef VulkanWindow_h
#define VulkanWindow_h

class Texture;

/**
	@brief A GLFW window containing a Vulkan surface
 */
class VulkanWindow
{
public:
	VulkanWindow(const std::string& title, vk::raii::Queue& queue, size_t queueFamily);
	virtual ~VulkanWindow();

	GLFWwindow* GetWindow()
	{ return m_window; }

	virtual void Render();

	vk::raii::Queue& GetRenderQueue()
	{ return m_renderQueue; }
	size_t GetRenderQueueFamily()
	{ return m_queueFamily; }

	void AddTextureUsedThisFrame(std::shared_ptr<Texture> tex)
	{ m_texturesUsedThisFrame.emplace(tex); }

protected:
	bool UpdateFramebuffer();
	void SetFullscreen(bool fullscreen);

	virtual void DoRender(vk::raii::CommandBuffer& cmdBuf);
	virtual void RenderUI();

	///@brief The underlying GLFW window object
	GLFWwindow* m_window;

	///@brief Surface for drawing onto
	std::shared_ptr<vk::raii::SurfaceKHR> m_surface;

	///@brief Descriptor pool for ImGui
	std::unique_ptr<vk::raii::DescriptorPool> m_imguiDescriptorPool;

	///@brief Queue for rendering to
	vk::raii::Queue& m_renderQueue;
	///@brief Family index of m_renderQueue
	size_t m_queueFamily;

	///@brief Set true if we have to handle a resize event
	bool m_resizeEventPending;

	///@brief Frame command pool
	std::unique_ptr<vk::raii::CommandPool> m_cmdPool;

	///@brief Frame command buffers
	std::vector<std::unique_ptr<vk::raii::CommandBuffer> > m_cmdBuffers;

	///@brief Semaphore indicating framebuffer is ready
	std::vector<std::unique_ptr<vk::raii::Semaphore> > m_imageAcquiredSemaphores;

	///@brief Semaphore indicating frame is complete
	std::vector<std::unique_ptr<vk::raii::Semaphore> > m_renderCompleteSemaphores;

	///@brief Frame semaphore number for double buffering
	uint32_t m_semaphoreIndex;

	///@brief Frame number for double buffering
	uint32_t m_frameIndex;

	///@brief Frame fences
	std::vector<std::unique_ptr<vk::raii::Fence> > m_fences;

	///@brief Back buffer view
	std::vector<std::unique_ptr<vk::raii::ImageView> > m_backBufferViews;

	///@brief Framebuffer
	std::vector<std::unique_ptr<vk::raii::Framebuffer> > m_framebuffers;

	///@brief Render pass for drawing everything
	std::unique_ptr<vk::raii::RenderPass> m_renderPass;

	///@brief Swapchain for presenting to the screen
	std::unique_ptr<vk::raii::SwapchainKHR> m_swapchain;

	///@brief Back buffer images
	std::vector<VkImage> m_backBuffers;

	///@brief Current window width
	int m_width;

	///@brief Current window height
	int m_height;

	///@brief Fullscreen flag
	bool m_fullscreen;

	///@brief Saved position before we went fullscreen
	int m_windowedX;

	///@brief Saved position before we went fullscreen
	int m_windowedY;

	///@brief Saved size before we went fullscreen
	int m_windowedWidth;

	///@brief Saved size before we went fullscreen
	int m_windowedHeight;

	///@brief implot context
	ImPlotContext* m_plotContext;

	///@brief Textures used this frame
	std::set<std::shared_ptr<Texture> > m_texturesUsedThisFrame;
};

#endif

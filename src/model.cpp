#include <model.hpp>
static const char* TAG = "Model";

static const size_t rawVertexbufferSize = 12 * 3 * sizeof(float);
static const float rawVertexBuffer[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static void dumpMat4(float *m4, const char* title){
    printf("----------------------------------------------\n");
    printf("\t%s\n",title);
    printf("----------------------------------------------\n");
    for(int a = 0; a < 16; a++){
        printf("%.2f\t", m4[a]);
        if(a % 4 == 3)
            printf("\n");
    }
    printf("----------------------------------------------\n");
}
  /*---------------------------------*/
 /*     Constructor & Destructor    */
/*---------------------------------*/
Model::Model(SVulkanMeta *meta)
{
    mMeta = meta;
}

Model::~Model(){
    if(!mMeta->deviceFunctions())
        return;

    //Free buffers
    if(vertexbuffer)
    delete vertexbuffer;
    vertexbuffer = nullptr;

    if(uniformbuffer)
        delete uniformbuffer;
    uniformbuffer = nullptr;

    if(mMeshPipelineLayout)
        mMeta->deviceFunctions()->vkDestroyPipelineLayout(mMeta->device(), mMeshPipelineLayout, NULL);

    //Free descriptor set
    //mMeta->deviceFunctions()->vkDestroyDescriptorSetLayout(mMeta->device(), mDescriptorSetLayout, NULL);
}
  /*---------------------------------*/
 /*        Member Functions         */
/*---------------------------------*/
void Model::setViewportSize(const QSize &size){
    mViewportSize = size;
}

uint32_t Model::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    mMeta->functions()->vkGetPhysicalDeviceMemoryProperties(mMeta->physicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    ErrorHandle::logE(TAG, "failed to find suitable memory type!");
    return 0;
}

VkResult Model::loadBuffers(){
    vertexbuffer = new SVkBuffer(mMeta);
    vertexbuffer->create(rawVertexbufferSize);
    vertexbuffer->writeData(rawVertexBuffer);
    ErrorHandle::logI(TAG, "Vertexbuffer setup sucessful!");

    uniformbuffer = new SVkBuffer(mMeta);
    uniformbuffer->create(sizeof(SUniformBufferObject));
    ErrorHandle::logI(TAG, "Uniformbuffer setup sucessful!");
    return VK_SUCCESS;
}

void Model::createConstatntsPipeline(){
    VkPipelineLayoutCreateInfo meshPipelineLayoutInfo{};
    meshPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    meshPipelineLayoutInfo.pNext = nullptr;

    //empty defaults
    meshPipelineLayoutInfo.flags = 0;
    meshPipelineLayoutInfo.setLayoutCount = 0;
    meshPipelineLayoutInfo.pSetLayouts = nullptr;

    VkPushConstantRange push_constant;
    push_constant.offset = 0;
    push_constant.size = sizeof(SUniformBufferObject);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    meshPipelineLayoutInfo.pushConstantRangeCount = 1;
    meshPipelineLayoutInfo.pPushConstantRanges = &push_constant;

    if(mMeta->deviceFunctions()->vkCreatePipelineLayout(mMeta->device(), &meshPipelineLayoutInfo, nullptr, &mMeshPipelineLayout) != VK_SUCCESS){
        ErrorHandle::logE(TAG, "Mesh PipelineLayout creation failed");
        return;
    }
}

void Model::createDescriptorSet(){
    VkResult err = VK_SUCCESS;

    VkDescriptorSetLayoutBinding descLayoutBinding;
    memset(&descLayoutBinding, 0, sizeof(descLayoutBinding));
    descLayoutBinding.binding = 0;
    descLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descLayoutBinding.descriptorCount = 1;
    descLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    VkDescriptorSetLayoutCreateInfo layoutInfo;
    memset(&layoutInfo, 0, sizeof(layoutInfo));
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &descLayoutBinding;
    err = mMeta->deviceFunctions()->vkCreateDescriptorSetLayout(mMeta->device(), &layoutInfo, nullptr, &mDescriptorSetLayout);
    if (err != VK_SUCCESS)
        qFatal("Failed to create descriptor set layout: %d", err);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;
    err = mMeta->deviceFunctions()->vkCreatePipelineLayout(mMeta->device(), &pipelineLayoutInfo, nullptr, &mDescriptorPipelineLayout);
    if (err != VK_SUCCESS)
        ErrorHandle::logW(TAG,"Failed to create pipeline layout");

    VkDescriptorPoolSize descPoolSizes[] = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1 }
    };
    VkDescriptorPoolCreateInfo descPoolInfo;
    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descPoolInfo.flags = 0; // won't use vkFreeDescriptorSets
    descPoolInfo.maxSets = 1;
    descPoolInfo.poolSizeCount = sizeof(descPoolSizes) / sizeof(descPoolSizes[0]);
    descPoolInfo.pPoolSizes = descPoolSizes;
    err = mMeta->deviceFunctions()->vkCreateDescriptorPool(mMeta->device(), &descPoolInfo, nullptr, &mDescriptorPool);
    if (err != VK_SUCCESS)
        qFatal("Failed to create descriptor pool: %d", err);

    VkDescriptorSetAllocateInfo descAllocInfo;
    memset(&descAllocInfo, 0, sizeof(descAllocInfo));
    descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descAllocInfo.descriptorPool = mDescriptorPool;
    descAllocInfo.descriptorSetCount = 1;
    descAllocInfo.pSetLayouts = &mDescriptorSetLayout;
    err = mMeta->deviceFunctions()->vkAllocateDescriptorSets(mMeta->device(), &descAllocInfo, &mDescriptorSet);
    if (err != VK_SUCCESS)
        qFatal("Failed to allocate descriptor set");

    VkWriteDescriptorSet writeInfo;
    memset(&writeInfo, 0, sizeof(writeInfo));
    writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeInfo.dstSet = mDescriptorSet;
    writeInfo.dstBinding = 0;
    writeInfo.descriptorCount = 1;
    writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

    VkDescriptorBufferInfo bufInfo;
    bufInfo.buffer = *(uniformbuffer->buffer());
    bufInfo.offset = 0; // dynamic offset is used so this is ignored
    bufInfo.range = sizeof(SUniformBufferObject);
    writeInfo.pBufferInfo = &bufInfo;
    mMeta->deviceFunctions()->vkUpdateDescriptorSets(mMeta->device(), 1, &writeInfo, 0, nullptr);
}

void Model::updateUniformBuffer(){
    QMatrix4x4 model, view, proj;
    QVector3D eye(4, 3, 3);
    QVector3D direction(0, 0, 0);
    QVector3D up(0, 1, 0);

    model.setToIdentity();
    view.lookAt(eye, direction, up);
    proj.perspective(
                qDegreesToRadians(45.0f),
                4.0f/3.0f,
                0.1f,
                100.0f
    );

    //Copy Matrices into buffer
    memcpy(ubo.model, model.data(), S_MAT4X4_SIZE_RAW);
    memcpy(ubo.view, view.data(), S_MAT4X4_SIZE_RAW);
    memcpy(ubo.proj, proj.data(), S_MAT4X4_SIZE_RAW);

    dumpMat4((model * view * proj).data(), "Model");
    //uniformbuffer->writeData(&ubo);
}

void Model::prepareShader(Stage stage, QString filename){
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)){
        qFatal("Failed to read shader %s", qPrintable(filename));
    }

    const QByteArray contents = f.readAll();

    if (stage == VertexStage) {
        mVert = contents;
        Q_ASSERT(!mVert.isEmpty());
    } else {
        mFrag = contents;
        Q_ASSERT(!mFrag.isEmpty());
    }
}

void Model::init(){
    memoryInformation();
    createGraphicsPipeline();
}

void Model::memoryInformation(){

}

void Model::createGraphicsPipeline(){
    mInitialized = true;

    VkPipelineCacheCreateInfo pipelineCacheInfo;
    memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    if(mMeta->deviceFunctions()->vkCreatePipelineCache(mMeta->device(), &pipelineCacheInfo, nullptr, &mPipelineCache) != VK_SUCCESS){
        ErrorHandle::logE(TAG, "Failed to create pipeline cache");
        abort();
    }

    VkShaderModule vertShaderModule = createShaderModule(&mVert);
    VkShaderModule fragShaderModule = createShaderModule(&mFrag);

    //Create Main Struct
    VkGraphicsPipelineCreateInfo pipelineInfo;
    memset(&pipelineInfo, 0, sizeof(pipelineInfo));
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    //add a Create Info for each shader
    VkPipelineShaderStageCreateInfo stageInfo[2];
    memset(&stageInfo, 0, sizeof(stageInfo));
    stageInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stageInfo[0].module = vertShaderModule;
    stageInfo[0].pName = "main";
    stageInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stageInfo[1].module = fragShaderModule;
    stageInfo[1].pName = "main";
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stageInfo;

    auto vertexBinding = bindingDescription();
    auto vertexAttr = attributeDescription();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    memset(&vertexInputInfo, 0, sizeof(vertexInputInfo));
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &vertexBinding;
    vertexInputInfo.vertexAttributeDescriptionCount = 1;
    vertexInputInfo.pVertexAttributeDescriptions = &vertexAttr;
    pipelineInfo.pVertexInputState = &vertexInputInfo;

    VkDynamicState dynStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicInfo;
    memset(&dynamicInfo, 0, sizeof(dynamicInfo));
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount = 2;
    dynamicInfo.pDynamicStates = dynStates;
    pipelineInfo.pDynamicState = &dynamicInfo;

    VkPipelineViewportStateCreateInfo viewportInfo;
    memset(&viewportInfo, 0, sizeof(viewportInfo));
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = viewportInfo.scissorCount = 1;
    pipelineInfo.pViewportState = &viewportInfo;

    VkPipelineInputAssemblyStateCreateInfo iaInfo;
    memset(&iaInfo, 0, sizeof(iaInfo));
    iaInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    iaInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    pipelineInfo.pInputAssemblyState = &iaInfo;

    VkPipelineRasterizationStateCreateInfo rsInfo;
    memset(&rsInfo, 0, sizeof(rsInfo));
    rsInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rsInfo.lineWidth = 1.0f;
    pipelineInfo.pRasterizationState = &rsInfo;

    VkPipelineMultisampleStateCreateInfo msInfo;
    memset(&msInfo, 0, sizeof(msInfo));
    msInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    msInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineInfo.pMultisampleState = &msInfo;

    VkPipelineDepthStencilStateCreateInfo dsInfo;
    memset(&dsInfo, 0, sizeof(dsInfo));
    dsInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineInfo.pDepthStencilState = &dsInfo;

    // SrcAlpha, One
    VkPipelineColorBlendStateCreateInfo blendInfo;
    memset(&blendInfo, 0, sizeof(blendInfo));
    blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    VkPipelineColorBlendAttachmentState blend;
    memset(&blend, 0, sizeof(blend));
    blend.blendEnable = true;
    blend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
    blend.colorBlendOp = VK_BLEND_OP_ADD;
    blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    blend.alphaBlendOp = VK_BLEND_OP_ADD;
    blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
            | VK_COLOR_COMPONENT_A_BIT;
    blendInfo.attachmentCount = 1;
    blendInfo.pAttachments = &blend;
    pipelineInfo.pColorBlendState = &blendInfo;

    pipelineInfo.layout = mPipelineLayout;

    pipelineInfo.renderPass = mMeta->renderPass();

    if(mMeta->deviceFunctions()->vkCreateGraphicsPipelines(
                mMeta->device(),
                mPipelineCache,
                1,
                &pipelineInfo,
                nullptr,
                &mGraphicsPipeline
                )
            ){
        ErrorHandle::logE(TAG, "Failed to create graphics pipeline");
        abort();
    }
    mMeta->deviceFunctions()->vkDestroyShaderModule(mMeta->device(), vertShaderModule, nullptr);
    mMeta->deviceFunctions()->vkDestroyShaderModule(mMeta->device(), fragShaderModule, nullptr);

    ErrorHandle::logI(TAG, "Graphics Pipelise setup sucessful!");
}

void Model::frameStart(){
    QSGRendererInterface *rif = mMeta->window()->rendererInterface();
    //If the render interface isn't Vulkan, abort program
    Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::VulkanRhi);

    if (mVert.isEmpty())
        prepareShader(VertexStage, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/testConst.vert.spv");
    if (mFrag.isEmpty())
        prepareShader(FragmentStage, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/test.frag.spv");

    if(!mMeta->isInit())
        mMeta->init(mMeta->window()->graphicsStateInfo().framesInFlight);
    if(!mInitialized)
        init();
    if(!mBuffersLoaded){
        mBuffersLoaded = true;
        loadBuffers();
        //createDescriptorSet();
        createConstatntsPipeline();
    }
}

void Model::mainPassRecordingStart(){
    const QQuickWindow::GraphicsStateInfo &stateInfo(mMeta->window()->graphicsStateInfo());
    updateUniformBuffer();
    QSGRendererInterface *rif = mMeta->window()->rendererInterface();
    // Must query the command buffer _after_ beginExternalCommands(), this is
    // actually important when running on Vulkan because what we get here is a
    // new secondary command buffer, not the primary one.
    mMeta->window()->beginExternalCommands();

    VkCommandBuffer cb = *reinterpret_cast<VkCommandBuffer *>(
    rif->getResource(mMeta->window(), QSGRendererInterface::CommandListResource));
    Q_ASSERT(cb);

    mMeta->deviceFunctions()->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);

    VkDeviceSize vbufOffset = 0;
    mMeta->deviceFunctions()->vkCmdBindVertexBuffers(cb, 0, 1, vertexbuffer->buffer(), &vbufOffset);

    VkViewport vp = { 0, 0, float(mViewportSize.width()), float(mViewportSize.height()), 0.0f, 1.0f };
    mMeta->deviceFunctions()->vkCmdSetViewport(cb, 0, 1, &vp);
    VkRect2D scissor = { { 0, 0 }, { uint32_t(mViewportSize.width()), uint32_t(mViewportSize.height()) } };
    mMeta->deviceFunctions()->vkCmdSetScissor(cb, 0, 1, &scissor);

    mMeta->deviceFunctions()->vkCmdPushConstants(cb, mMeshPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SUniformBufferObject), &ubo);
    mMeta->deviceFunctions()->vkCmdDraw(cb, 12, 1, 0, 0);

    mMeta->window()->endExternalCommands();
}
  /*---------------------------------*/
 /*        Static Functions         */
/*---------------------------------*/
VkVertexInputBindingDescription Model::bindingDescription(){
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(QVector3D);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

VkVertexInputAttributeDescription Model::attributeDescription(){
    VkVertexInputAttributeDescription viad{};
    memset(&viad, 0, sizeof(viad));
    viad.binding = 0;
    viad.location = 1;
    //use 3 * 4byte values as float. aka QVector3D
    viad.format = VK_FORMAT_R32G32B32_SFLOAT;
    viad.offset = 0;
    return viad;
}

VkShaderModule Model::createShaderModule(QByteArray *code){
    VkShaderModuleCreateInfo ci{};
    memset(&ci, 0, sizeof(ci));
    ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ci.codeSize = code->size();
    ci.pCode = reinterpret_cast<const quint32 *>(code->constData());

    VkShaderModule shaderModule;
    if (mMeta->deviceFunctions()->vkCreateShaderModule(mMeta->device(), &ci, nullptr, &shaderModule) != VK_SUCCESS){
        ErrorHandle::logE(TAG, "failed to create shader module!");
        abort();
    }
    return shaderModule;
}

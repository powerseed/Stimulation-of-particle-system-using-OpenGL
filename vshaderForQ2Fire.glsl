#version 150

in vec4 vPositionForFire;
in vec4 vColorForFire;
flat out vec4 colorForFire;
uniform mat4 ModelViewForFire, ProjectionForFire;

void main()
{
    gl_Position = ProjectionForFire * ModelViewForFire * vPositionForFire;

    vec4 model_Position = ModelViewForFire * vPositionForFire;

    colorForFire = vColorForFire;

    //float distanceFromCenter = sqrt( model_Position.x * model_Position.x + model_Position.y * model_Position.y + (model_Position.z - (-2)) * (model_Position.z - (-2)) );
    float distanceFromCenter = distance(model_Position, vec4(0.0f, -0.35f, -2.0f, 1));

    colorForFire.x = colorForFire.x + distanceFromCenter / 2.0f;
    colorForFire.y = colorForFire.y - distanceFromCenter / 2.0f;
}